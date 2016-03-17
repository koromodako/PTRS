#include "disconnectedstate.h"
#include "src/network/clientsession.h"

DisconnectedState::DisconnectedState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("DisconnectedState");
}

DisconnectedState::~DisconnectedState()
{
}

void DisconnectedState::ProcessHello()
{
    _client->send(OK, _client->GetId().toString());
    _client->setCurrentStateAfterSuccess();
}
