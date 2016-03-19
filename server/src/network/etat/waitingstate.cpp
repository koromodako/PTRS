#include "waitingstate.h"
#include "src/network/clientsession.h"

#include "src/utils/logger.h"

WaitingState::WaitingState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("WaitingState");
}

WaitingState::~WaitingState()
{
}

void WaitingState::ProcessReady(const QByteArray &content)
{
    LOG_DEBUG(QString("Testing '%1' against '%2'").arg(QString(content)).arg(_client->GetId().toString()));
    if (content == _client->GetId().toString())
        _client->setCurrentStateAfterSuccess();
}
