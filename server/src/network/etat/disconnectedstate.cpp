#include "disconnectedstate.h"
#include "src/network/clientsession.h"

DisconnectedState::DisconnectedState(ClientSession *parent) : AbstractState(parent)
{

}

DisconnectedState::~DisconnectedState()
{
}

void DisconnectedState::ProcessHello()
{
    _client->SendCmd(OK, _client->GetId().toString());
    _client->SetCurrentStateAfterSuccess();
}
