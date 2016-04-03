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

void AbstractState::ProcessAbort()
{
}

void AbstractState::ProcessDo(const QByteArray &content)
{
    Q_UNUSED(content)
}

void AbstractState::ProcessBin(const QByteArray &content)
{
    Q_UNUSED(content)
}

void AbstractState::ProcessDone(const QJsonObject &args)
{
    Q_UNUSED(args)
}

void AbstractState::ProcessHello()
{
}

void AbstractState::ProcessOK(const QByteArray &content)
{
    Q_UNUSED(content)
}

void AbstractState::ProcessStop()
{
}
