#include "workingabouttostartstate.h"
#include "src/network/networkmanager.h"
#include "src/network/clientsession.h"

WorkingAboutToStartState::WorkingAboutToStartState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("WorkingAboutToStartState");
}

WorkingAboutToStartState::~WorkingAboutToStartState()
{
}

void WorkingAboutToStartState::ProcessUnable(const QByteArray &content)
{
    if (content == _client->GetId().toString())
    {
        _client->AddMissingPlugin();
        _client->_fragment = NULL;
        _client->setCurrentStateAfterError("Unable to calculate");
    }
}

void WorkingAboutToStartState::ProcessWorking(const QByteArray &content)
{
    if (content == _client->GetId().toString())
        _client->setCurrentStateAfterSuccess();
}
