#include "waitingstate.h"
#include "src/network/clientsession.h"

WaitingState::WaitingState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("WaitingState");
}

WaitingState::~WaitingState()
{
}

void WaitingState::ProcessReady(const QStringList &args)
{
    if (args.size() > 0 && args.first() == _client->GetId().toString())
        _client->setCurrentStateAfterSuccess();
}
