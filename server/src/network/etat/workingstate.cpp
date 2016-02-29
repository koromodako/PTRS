#include "workingstate.h"
#include "src/network/clientsession.h"
#include "src/network/networkmanager.h"
#include <QJsonDocument>

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
        emit NetworkManager::getInstance().sig_calculAborted(_client->FragmentId());
        _client->SetCurrentStateAfterError(CALCUL_ABORTED);
    }
}

void WorkingState::ProcessDone(const QStringList &args)
{
    if (args.size() > 1 && args.first() == _client->GetId().toString())
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(args[1].toUtf8());
        emit NetworkManager::getInstance().sig_calculDone(_client->FragmentId(), jsonResponse.object());
        _client->SetCurrentStateAfterSuccess();
    }
}

void WorkingState::ProcessStop()
{
    _client->SendCmd(STOP, "");
    _client->SetCurrentStateAfterSuccess();
}
