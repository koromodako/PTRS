#include "clientsession.h"
#include "src/network/etat/disconnectedstate.h"
#include "src/network/etat/readystate.h"
#include "src/network/etat/waitingstate.h"
#include "src/network/etat/workingstate.h"
#include "src/plugins/pluginmanager.h"

#include <QJsonObject>
#include <QNetworkInterface>

static const unsigned broadcastPort = 45000;

ClientSession::ClientSession()
{
    _broadcastSocket = new QUdpSocket(this);
    _socket = new QTcpSocket(this);

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


void ClientSession::slot_processCmd(ReqType reqType, const QStringList &args)
{
    switch (reqType)
    {
        case OK:
            _currentState->ProcessOK(args);
            break;
        case DO:
            _currentState->ProcessDo(args);
            break;
        case STOP:
            _currentState->ProcessStop();
            break;
        default:
            qDebug() << "Impossible de traiter cette requète : " << QString::number(reqType);
            break;
    }
}

void ClientSession::slot_processReadyRead()
{
    if (_socket->bytesAvailable() <= 0)
        return;

    QByteArray d(_socket->readAll());
    QList<QString> datagrams = QString(d).split("#@@#", QString::SkipEmptyParts);
    foreach (QString datagram, datagrams)
    {
        QList<QString> args = datagram.split("##");

        ReqType reqType = (ReqType)args.takeFirst().toInt();
        slot_processCmd(reqType, args);
    }
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
            _socket->waitForConnected();
            _currentState->ProcessHello();

            return;
        }
    }
}

void ClientSession::SendCmd(ReqType reqType, const QString &args)
{
    QByteArray data = QByteArray::number(reqType) + "##";
    data += _id;
    if (args.size() > 0)
        data += "##" + args.toUtf8();
    data += "#@@#";

    _socket->write(data);
    _socket->flush();
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

void ClientSession::Slot_sendResultToServer(QJsonObject args)
{
    _currentState->ProcessDone(args);
}
