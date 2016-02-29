#include "readystate.h"
#include "src/network/networkmanager.h"

#include <QJsonObject>
#include <QJsonDocument>

ReadyState::ReadyState(ClientSession *parent) : AbstractState(parent)
{

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

void ReadyState::ProcessDo(const QJsonObject &args)
{
    QJsonDocument doc(args);
    _client->SendCmd(DO, doc.toJson(QJsonDocument::Compact));
    _client->SetCurrentStateAfterSuccess();
}
