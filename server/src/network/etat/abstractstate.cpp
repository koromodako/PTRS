#include "abstractstate.h"
#include "src/network/clientsession.h"

AbstractState::AbstractState(ClientSession *parent) : QObject(parent)
{
    _client = parent;
}

AbstractState::~AbstractState()
{

}

void AbstractState::OnEntry()
{

}

void AbstractState::OnExit()
{

}

void AbstractState::ProcessAbort(const QStringList &args)
{
    Q_UNUSED(args)
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessDo(const QJsonObject &args)
{
    Q_UNUSED(args)
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessDone(const QStringList &args)
{
    Q_UNUSED(args)
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessHello()
{
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessReady(const QStringList &args)
{
    Q_UNUSED(args)
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessStop()
{
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessUnable(const QStringList &args)
{
    Q_UNUSED(args)
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}

void AbstractState::ProcessWorking(const QStringList &args)
{
    Q_UNUSED(args)
    _client->SetCurrentStateAfterError(ACTION_NOT_HANDlED);
}
