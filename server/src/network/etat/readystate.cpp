#include "readystate.h"
#include "src/network/networkmanager.h"

ReadyState::ReadyState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("ReadyState");
}

ReadyState::~ReadyState()
{
}


void ReadyState::OnEntry()
{
    emit _client->sig_ready(_client);
}

void ReadyState::OnExit()
{
    emit _client->sig_working(_client);
}

void ReadyState::ProcessDo(const QString &args)
{
    _client->sendCmd(DO, args);
    _client->setCurrentStateAfterSuccess();
}
