#include "workingstate.h"
#include "src/network/clientsession.h"
#include "src/network/networkmanager.h"
#include "src/utils/logger.h"

WorkingState::WorkingState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("WorkingState");
}

WorkingState::~WorkingState()
{
}

void WorkingState::ProcessAbort(const QByteArray &content)
{
    if (content == _client->GetId().toString())
    {
        _client->resetCurrentFragment();
        _client->setCurrentStateAfterError("CalculAborted");
    }
}

void WorkingState::ProcessDone(const QByteArray &content)
{
    LOG_DEBUG("expected : " + _client->GetId().toString().toUtf8());
    LOG_DEBUG("receive : " + content);
    if (content.startsWith(_client->GetId().toString().toUtf8()))
    {
        emit _client->sig_calculDone(content.mid(_client->GetId().toString().size()));
        _client->resetCurrentFragment();
        _client->setCurrentStateAfterSuccess();
    }
}

void WorkingState::ProcessStop()
{
    _client->send(STOP);
    _client->setCurrentStateAfterSuccess();
}
