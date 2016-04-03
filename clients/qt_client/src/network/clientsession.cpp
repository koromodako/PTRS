#include "clientsession.h"
#include "src/network/etat/disconnectedstate.h"
#include "src/network/etat/readystate.h"
#include "src/network/etat/waitingstate.h"
#include "src/network/etat/workingstate.h"
#include "src/plugins/pluginmanager.h"
#include "src/utils/logger.h"

#include <QJsonObject>
#include <QNetworkInterface>
#include <QDataStream>

static const unsigned broadcastPort = 45000;

/// Ce type est celui utilisé pour stocker la commande associée à un message
typedef quint8  req_t;

ClientSession::ClientSession() :
    _blockSize(0)
{
    _currentCalculation = NULL;
    _broadcastSocket = new QUdpSocket(this);
    _socket = new QTcpSocket(this);
    connect(this, &ClientSession::sig_requestCalculStart, &PluginManager::getInstance(), &PluginManager::Slot_calc);
    connect(this, &ClientSession::sig_requestCalculStop, &PluginManager::getInstance(), &PluginManager::Slot_stop);
    connect(_socket, &QTcpSocket::readyRead, this, &ClientSession::slot_processReadyRead);
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_disconnect()));
    initializeStateMachine();

    _broadcastSocket->bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress
                         | QUdpSocket::ReuseAddressHint);

    _broadcastTimer.setInterval(2000);
    connect(&_broadcastTimer, &QTimer::timeout, this, &ClientSession::findServer);

    findServer();
}

ClientSession::~ClientSession()
{
    _socket->deleteLater();
}

void ClientSession::Slot_abortCalcul()
{
    _currentState->ProcessAbort();
}

void ClientSession::slot_disconnect()
{
    _currentState->OnExit();
    _currentState = _disconnectedState;
    _currentState->OnEntry();
    findServer();
}

void ClientSession::findServer()
{
    _broadcastTimer.start(_broadcastTimer.interval());
    connect(_broadcastSocket, &QUdpSocket::readyRead, this, &ClientSession::readBroadcastDatagram);

    QByteArray datagram = QByteArray::number(HELLO) + "#@@#";

    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
            if (entry.broadcast() != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost)
                _broadcastSocket->writeDatagram(datagram, entry.broadcast(), broadcastPort);
        }
    }
}

void ClientSession::initializeStateMachine()
{
    DisconnectedState *disconnectedState = new DisconnectedState(this);
    ReadyState *readyState = new ReadyState(this);
    WaitingState *waitingState = new WaitingState(this);
    WorkingState *workingState = new WorkingState(this);

    _transitionsMap[disconnectedState] = waitingState;
    _transitionsMap[waitingState] = readyState;
    _transitionsMap[readyState] = workingState;
    _transitionsMap[workingState] = readyState;

    _currentState = _disconnectedState = disconnectedState;
}


void ClientSession::slot_processRequest(ReqType reqType, const QByteArray &content)
{
    switch (reqType)
    {
        case OK:
            LOG_DEBUG("processing OK request");
            _currentState->ProcessOK(content);
            break;
        case DO:
            LOG_DEBUG("processing DO request");
            _currentState->ProcessDo(content);
            break;
        case STOP:
            LOG_DEBUG("processing STOP request");
            _currentState->ProcessStop();
            break;
        case BIN:
            LOG_DEBUG("processing BIN request");
            _currentState->ProcessBin(content);
            break;
        default:
            qDebug() << "Impossible de traiter cette requète : " << QString::number(reqType);
            break;
    }
}

void ClientSession::slot_processReadyRead()
{
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_5_3);

    if(_blockSize == 0)
    {   if(_socket->bytesAvailable() < (int)(sizeof(msg_size_t)))
        {   return; // on a pas encore reçu suffisament d'octets pour connaitre la taille du message et la commande
        }
        // sinon on inscrit la taille du message dans l'attribut de la classe et la commande
        in >> _blockSize;
        LOG_DEBUG(QString("block size received : size=%1").arg(_blockSize));
    }

    if(_socket->bytesAvailable() < _blockSize)
        return; // on a pas encore reçu tout le message

    // on commence par récupérer la commande
    req_t req;
    in >> req;
    LOG_DEBUG(QString("request received : req=%1").arg(req));
    // on récupère ensuite le contenu du message que l'on passe au slot de traitement
    QByteArray content;
    in >> content;
    LOG_DEBUG(QString("content received : text=").append(content));
    slot_processRequest((ReqType)req, content);
    // on reset les variables commande et block size
    _blockSize = 0;
}

void ClientSession::readBroadcastDatagram()
{
    while (_broadcastSocket->hasPendingDatagrams())
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray d;
        d.resize(_broadcastSocket->pendingDatagramSize());
        if (_broadcastSocket->readDatagram(d.data(), d.size(), &senderIp, &senderPort) == -1)
            continue;

        QList<QString> datagrams = QString(d).split("#@@#", QString::SkipEmptyParts);
        foreach (QString datagram, datagrams)
        {
            QList<QString> data = datagram.split("##");

            //If there are not enough argument, we don't process the packet.
            if (data.size() < 2 || data.first().toInt() != HELLO_FROM_SERVER)
                continue;

            _broadcastTimer.stop();
            QObject::disconnect(_broadcastSocket, &QUdpSocket::readyRead, this, &ClientSession::readBroadcastDatagram);
            _socket->connectToHost(senderIp, data[1].toInt());
            LOG_INFO("connected to " + senderIp.toString());
            _socket->waitForConnected();
            _currentState->ProcessHello();

            return;
        }
    }
}

void ClientSession::Send(ReqType reqType, const QString &content)
{
    LOG_DEBUG(QString("Send(reqType='%1',content='%2') called").arg((int)reqType).arg(content));
    // vérification de la taille du contenu à envoyer en octets
    if( (content.toUtf8().size()+sizeof(req_t)) > MSG_SIZE_MAX)
    {   LOG_ERROR("Message content too long to be sent !");
        return;
    }

    QByteArray block("");                           // on crée le bloc de données
    QDataStream out(&block, QIODevice::WriteOnly);  // on crée un datastream pour normaliser le bloc
    out.setVersion(QDataStream::Qt_5_3);            // on donne la version du datastream pour spécifier la normalisation

    out << (msg_size_t)0;                                   // on reserve sizeof(msg_size_t) pour stocker la taille du message
    out << (req_t)reqType;                                  // on écrit dans le champs requete
    out << content.toUtf8();                                // on écrit le contenu après la requete
    out.device()->seek(0);                                  // on déplace la tête d'écriture au début
    out << (msg_size_t)(block.size() - (int)sizeof(msg_size_t)); // on écrit la taille du message (commande comprise)

    LOG_DEBUG(QString("writing block into socket : block=").append(block.toHex()));

    _socket->write(block);  // on écrit le bloc dans le socket
    _socket->flush();       // on flush le socket
}

void ClientSession::Slot_startCalculation(Calculation *calculation)
{
    if (_currentCalculation != NULL)
    {
        LOG_DEBUG("Un calcul est déjà en cours");
        return;
    }
    _currentCalculation = calculation;
    connect(_currentCalculation, &Calculation::sig_computed, this, &ClientSession::Slot_sendResultToServer);
    connect(_currentCalculation, &Calculation::sig_canceled, this, &ClientSession::Slot_abortCalcul);
    connect(_currentCalculation, &Calculation::sig_crashed, this, &ClientSession::Slot_abortCalcul);

    emit sig_requestCalculStart(calculation);
}

void ClientSession::SetCurrentState()
{
    QMap<QObject *, AbstractState *>::const_iterator it = _transitionsMap.find(_currentState);
    if (it != _transitionsMap.end())
    {
        _currentState->OnExit();
        _currentState = it.value();
        _currentState->OnEntry();
    }
}

void ClientSession::setId(const QString &id)
{
    _id = id;
}

void ClientSession::Slot_sendResultToServer()
{
    if (_currentCalculation == NULL)
    {
        LOG_DEBUG("Aucun calcul n'est en cours");
        return;
    }
    _currentState->ProcessDone(_currentCalculation->GetResult());
    _currentCalculation->deleteLater();
    _currentCalculation = NULL;
}
