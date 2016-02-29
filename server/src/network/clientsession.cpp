#include "clientsession.h"
#include "src/network/etat/disconnectedstate.h"
#include "src/network/etat/readystate.h"
#include "src/network/etat/waitingstate.h"
#include "src/network/etat/workingabouttostartstate.h"
#include "src/network/etat/workingstate.h"
#include "src/utils/logger.h"

#include <QJsonObject>

ClientSession::ClientSession(QTcpSocket *associatedSocket, QObject *parent) : AbstractIdentifiable(parent)
{
    _socket = associatedSocket;

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
    _currentState->OnEntry();}

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

void ClientSession::slot_processCmd(ReqType reqType, const QStringList &args)
{
    switch (reqType)
    {
        case HELLO:
            _currentState->ProcessHello();
            break;
        case READY:
            _currentState->ProcessReady(args);
            break;
        case WORKING:
            _currentState->ProcessWorking(args);
            break;
        case UNABLE:
            _currentState->ProcessUnable(args);
            break;
        case DONE:
            _currentState->ProcessDone(args);
            break;
        case ABORT:
            _currentState->ProcessAbort(args);
            break;
        default:
            LOG_DEBUG(QString("Impossible de traiter cette requète : " + QString::number(reqType)));
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

void ClientSession::SendCmd(ReqType reqType, const QString &args)
{
    QByteArray data = QByteArray::number(reqType);
    if (args.size() > 0)
        data +=  "##" + args.toUtf8();
    data += "#@@#";

    _socket->write(data);
    _socket->flush();
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

void ClientSession::SetCurrentStateAfterError(ErrorCode errorCode)
{
    LOG_ERROR("Erreur in state " + _currentState->objectName() + " : " + QString::number(errorCode));

    setCurrentState(_errorTransitionsMap);
}

void ClientSession::SetCurrentStateAfterSuccess()
{
    setCurrentState(_doneTransitionsMap);
}

void ClientSession::StartCalcul(int fragmentId, QJsonObject args)
{
    _fragmentId = fragmentId;
    _currentState->ProcessDo(args);
}

void ClientSession::StopCalcul()
{
    if (_fragmentId == -1)
        return;
    _currentState->ProcessStop();
    _fragmentId = -1;
}
