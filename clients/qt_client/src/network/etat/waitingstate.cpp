#include "waitingstate.h"
#include "src/network/clientsession.h"

WaitingState::WaitingState(ClientSession *parent) : AbstractState(parent)
{

}

WaitingState::~WaitingState()
{
}

void WaitingState::ProcessOK(const QStringList &args)
{
    if (args.size() > 0)
    {
        _client->setId(args.first());
        _client->SendCmd(READY, "");
        _client->SetCurrentState();
    }
}
