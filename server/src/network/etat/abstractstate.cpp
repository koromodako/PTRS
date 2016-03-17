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

void AbstractState::ProcessAbort(const QByteArray &content)
{
    Q_UNUSED(content)
    _client->setCurrentStateAfterError("Abort not handled");
}

void AbstractState::ProcessDo(const QByteArray &content)
{
    Q_UNUSED(content)
    _client->setCurrentStateAfterError("Do not handled");
}

void AbstractState::ProcessDone(const QByteArray &content)
{
    Q_UNUSED(content)
    _client->setCurrentStateAfterError("Done not handled");
}

void AbstractState::ProcessHello()
{
    _client->setCurrentStateAfterError("Hello not handled");
}

void AbstractState::ProcessReady(const QByteArray &content)
{
    Q_UNUSED(content)
    _client->setCurrentStateAfterError("Ready not handled");
}

void AbstractState::ProcessStop()
{
    _client->setCurrentStateAfterError("stop not handled");
}

void AbstractState::ProcessUnable(const QByteArray &content)
{
    Q_UNUSED(content)
    _client->setCurrentStateAfterError("Unable not handled");
}

void AbstractState::ProcessWorking(const QByteArray &content)
{
    Q_UNUSED(content)
    _client->setCurrentStateAfterError("Working not handled");
}
