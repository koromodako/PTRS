#include "workingabouttostartstate.h"
#include "src/network/networkmanager.h"
#include "src/network/clientsession.h"

WorkingAboutToStartState::WorkingAboutToStartState(ClientSession *parent) : AbstractState(parent)
{

}

WorkingAboutToStartState::~WorkingAboutToStartState()
{
}

void WorkingAboutToStartState::ProcessUnable(const QStringList &args)
{
    //TODO: transmettre à networkManager pour qu'il transmette le calcul à un autre client
    if (args.size() > 0 && args.first() == _client->GetId().toString())
    {
        _client->_fragment = NULL;
        _client->setCurrentStateAfterError(UNABLE_TO_CALCULATE);
    }
}

void WorkingAboutToStartState::ProcessWorking(const QStringList &args)
{
    if (args.size() > 0 && args.first() == _client->GetId().toString())
        _client->setCurrentStateAfterSuccess();
}
