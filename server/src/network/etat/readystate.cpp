#include "readystate.h"
#include "src/network/networkmanager.h"
#include "src/utils/logger.h"

ReadyState::ReadyState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("ReadyState");
}

ReadyState::~ReadyState()
{
}


void ReadyState::OnEntry()
{
    LOG_DEBUG("Emitting sig_ready");
    emit _client->sig_ready(_client);
}

void ReadyState::OnExit()
{
    emit _client->sig_working(_client);
}

void ReadyState::ProcessDo(const QByteArray &content)
{
    _client->send(DO, content);
    _client->setCurrentStateAfterSuccess();
}
