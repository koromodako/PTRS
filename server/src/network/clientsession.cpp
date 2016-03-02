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
    _fragment = NULL;

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

void ClientSession::sendCmd(ReqType reqType, const QString &args)
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

void ClientSession::setCurrentStateAfterError(const QString &error)
{
    LOG_ERROR("Erreur in state " + _currentState->objectName() + " : " + error);
    setCurrentState(_errorTransitionsMap);
}

void ClientSession::setCurrentStateAfterSuccess()
{
    setCurrentState(_doneTransitionsMap);
}

bool ClientSession::StartCalcul(const Calculation *fragment, const QString &json)
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
    _currentState->ProcessDo(json);
    return true;
}

void ClientSession::Slot_stopCalcul()
{
    if (_fragment == NULL)
        return;
    _currentState->ProcessStop();
    _fragment = NULL;
}
