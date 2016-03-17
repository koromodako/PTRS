#include "waitingstate.h"
#include "src/network/clientsession.h"

WaitingState::WaitingState(ClientSession *parent) : AbstractState(parent)
{

}

WaitingState::~WaitingState()
{
}

void WaitingState::ProcessOK(const QByteArray &content)
{
    if (content.size() > 0)
    {
        _client->setId(content);
        _client->Send(READY, _client->Id());
        _client->SetCurrentState();
    }
}
