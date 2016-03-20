#include "clientsession.h"
#include "src/network/etat/disconnectedstate.h"
#include "src/network/etat/readystate.h"
#include "src/network/etat/waitingstate.h"
#include "src/network/etat/workingabouttostartstate.h"
#include "src/network/etat/workingstate.h"
#include "src/utils/logger.h"

#include <QJsonObject>
#include <QDataStream>

/// Ce type est celui utilisé pour stocker la commande associée à un message
typedef quint8  req_t;

ClientSession::ClientSession(QTcpSocket *associatedSocket, QObject *parent) :
    AbstractIdentifiable(parent),
    _fragment(NULL),
    _socket(associatedSocket),
    _blockSize(0)
{
    connect(_socket, &QTcpSocket::readyRead, this, &ClientSession::slot_processReadyRead);
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_disconnect()));
    initializeStateMachine();
}

ClientSession::~ClientSession()
{
    _socket->deleteLater();
}

void ClientSession::slot_disconnect()
{
    _currentState->OnExit();
    _currentState = _disconnectedState;
    _currentState->OnEntry();
    emit sig_disconnected(this);
}

void ClientSession::initializeStateMachine()
{
    DisconnectedState *disconnectedState = new DisconnectedState(this);
    ReadyState *readyState = new ReadyState(this);
    WaitingState *waitingState = new WaitingState(this);
    WorkingAboutToStartState *workingAboutToStartState = new WorkingAboutToStartState(this);
    WorkingState *workingState = new WorkingState(this);

    _doneTransitionsMap[disconnectedState] = waitingState;
    _doneTransitionsMap[waitingState] = readyState;
    _doneTransitionsMap[readyState] = workingAboutToStartState;
    _doneTransitionsMap[workingAboutToStartState] = workingState;
    _doneTransitionsMap[workingState] = readyState;

    _errorTransitionsMap[workingAboutToStartState] = readyState;
    _errorTransitionsMap[workingState] = readyState;

    _currentState = _disconnectedState = disconnectedState;
}

void ClientSession::slot_processRequest(ReqType reqType, const QByteArray & content)
{
    switch (reqType)
    {
        case HELLO:
            LOG_DEBUG("processing HELLO request");
            _currentState->ProcessHello();
            break;
        case READY:
            LOG_DEBUG("processing READY request");
            _currentState->ProcessReady(content);
            break;
        case WORKING:
            LOG_DEBUG("processing WORKING request");
            _currentState->ProcessWorking(content);
            break;
        case UNABLE:
            LOG_DEBUG("processing UNABLE request");
            _currentState->ProcessUnable(content);
            break;
        case DONE:
            LOG_DEBUG("processing DONE request");
            _currentState->ProcessDone(content);
            break;
        case ABORT:
            LOG_DEBUG("processing ABORT request");
            _currentState->ProcessAbort(content);
            break;
        default:
            LOG_DEBUG(QString("Impossible de traiter cette requète : " + QString::number(reqType)));
            break;
    }
}

void ClientSession::slot_processReadyRead()
{
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_5_5);

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

void ClientSession::send(ReqType reqType, const QString & content)
{
    LOG_DEBUG(QString("send(reqType='%1',content='%2') called").arg((int)reqType).arg(content));
    // vérification de la taille du contenu à envoyer en octets
    if( (content.toUtf8().size()+sizeof(req_t)) > MSG_SIZE_MAX)
    {   LOG_ERROR("Message content too long to be sent !");
        return;
    }

    QByteArray block;                           // on crée le bloc de données
    QDataStream out(&block, QIODevice::WriteOnly);  // on crée un datastream pour normaliser le bloc
    out.setVersion(QDataStream::Qt_5_5);            // on donne la version du datastream pour spécifier la normalisation

    out << (msg_size_t)0;                                   // on reserve sizeof(msg_size_t) pour stocker la taille du message
    out << (req_t)reqType;                                  // on écrit dans le champs requete
    out << content.toUtf8();                                // on écrit le contenu après la requete
    out.device()->seek(0);                                  // on déplace la tête d'écriture au début
    out << (msg_size_t)(block.size() - (int)sizeof(msg_size_t)); // on écrit la taille du message (commande comprise)

    LOG_DEBUG(QString("writing block into socket : block=").append(block.toHex()));

    _socket->write(block);  // on écrit le bloc dans le socket
    _socket->flush();       // on flush le socket
}

void ClientSession::setCurrentState(const QMap<QObject *, AbstractState *> &transitionsMap)
{
    QMap<QObject *, AbstractState *>::const_iterator it = transitionsMap.find(_currentState);
    if (it != transitionsMap.end())
    {
        _currentState->OnExit();
        _currentState = it.value();
        _currentState->OnEntry();
    }
}

void ClientSession::setCurrentStateAfterError(const QString &error)
{
    LOG_ERROR("Erreur in state " + _currentState->objectName() + " : " + error);
    setCurrentState(_errorTransitionsMap);
}

void ClientSession::setCurrentStateAfterSuccess()
{
    setCurrentState(_doneTransitionsMap);
}

bool ClientSession::StartCalcul(const Calculation *fragment)
{
    //Impossible de commencer un autre calcul quand il y en a un en cours
    if (fragment == NULL || _fragment != NULL)
        return false;

    if (_fragment != NULL)
    {
        disconnect(this, &ClientSession::sig_calculAborted, _fragment, &Calculation::Slot_crashed);
        disconnect(this, &ClientSession::sig_calculDone, _fragment, &Calculation::Slot_computed);
        disconnect(_fragment, &Calculation::sig_canceled, this, &ClientSession::Slot_stopCalcul);
    }

    connect(this, &ClientSession::sig_calculAborted, fragment, &Calculation::Slot_crashed);
    connect(this, &ClientSession::sig_calculDone, fragment, &Calculation::Slot_computed);
    connect(fragment, &Calculation::sig_canceled, this, &ClientSession::Slot_stopCalcul);

    _fragment = fragment;
    _currentState->ProcessDo(fragment->ToJson().toUtf8());
    return true;
}

void ClientSession::Slot_stopCalcul()
{
    if (_fragment == NULL)
        return;
    _currentState->ProcessStop();
    _fragment = NULL;
}
