#include "workingstate.h"
#include "src/network/clientsession.h"
#include "src/network/networkmanager.h"

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
        _client->_fragment = NULL;
        emit _client->sig_calculAborted("Client abort the calcul");
        _client->setCurrentStateAfterError("CalculAborted");
    }
}

#define QUUID_STRING_SIZE QUuid().toString().toUtf8().size()

void WorkingState::ProcessDone(const QByteArray &content)
{
    if (content.startsWith(_client->GetId().toString().toUtf8()))
    {
        _client->_fragment = NULL;
        emit _client->sig_calculDone(content.mid(QUUID_STRING_SIZE));
        _client->setCurrentStateAfterSuccess();
    }
}

void WorkingState::ProcessStop()
{
    _client->send(STOP);
    _client->setCurrentStateAfterSuccess();
}
