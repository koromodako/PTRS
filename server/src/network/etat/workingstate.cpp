#include "workingstate.h"
#include "src/network/clientsession.h"
#include "src/network/networkmanager.h"

WorkingState::WorkingState(ClientSession *parent) : AbstractState(parent)
{

}

WorkingState::~WorkingState()
{
}

void WorkingState::ProcessAbort(const QStringList &args)
{
    if (args.size() > 0 && args.first() == _client->GetId().toString())
    {
        _client->_fragment = NULL;
        emit _client->sig_calculAborted("Client abort the calcul");
        _client->setCurrentStateAfterError(CALCUL_ABORTED);
    }
}

void WorkingState::ProcessDone(const QStringList &args)
{
    if (args.size() > 1 && args.first() == _client->GetId().toString())
    {
        _client->_fragment = NULL;
        emit _client->sig_calculDone(args[1].toUtf8());
        _client->setCurrentStateAfterSuccess();
    }
}

void WorkingState::ProcessStop()
{
    _client->sendCmd(STOP, "");
    _client->setCurrentStateAfterSuccess();
}
