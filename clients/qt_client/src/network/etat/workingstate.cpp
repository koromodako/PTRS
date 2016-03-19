#include "workingstate.h"
#include "src/network/clientsession.h"
#include <QJsonDocument>

WorkingState::WorkingState(ClientSession *parent) : AbstractState(parent)
{

}

WorkingState::~WorkingState()
{
}

void WorkingState::ProcessAbort()
{
    _client->Send(ABORT);
    _client->SetCurrentState();
}

void WorkingState::ProcessDone(const QJsonObject &args)
{
    QJsonDocument doc(args);
    _client->Send(DONE, doc.toJson(QJsonDocument::Compact));
    _client->SetCurrentState();
}

void WorkingState::ProcessStop()
{
    emit _client->sig_requestCalculStop();
    _client->SetCurrentState();
}
