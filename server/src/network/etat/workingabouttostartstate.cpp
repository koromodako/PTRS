#include "workingabouttostartstate.h"
#include "src/network/networkmanager.h"
#include "src/network/clientsession.h"
#include "src/plugins/pluginmanager.h"

WorkingAboutToStartState::WorkingAboutToStartState(ClientSession *parent) : AbstractState(parent)
{
    setObjectName("WorkingAboutToStartState");
}

WorkingAboutToStartState::~WorkingAboutToStartState()
{
}

void WorkingAboutToStartState::ProcessUnable(const QByteArray &content)
{
    // parsing du json reçu
    QJsonDocument doc;
    QJsonParseError error;
    doc.fromJson(content, &error);
    QJsonObject object = doc.object();
    // vérification de l'intégrité de l'objet JSON
    if(!object.contains("id") ||
       !object.contains("arch") ||
       !object.contains("os"))
    {
        _client->AddMissingPlugin();
        _client->resetCurrentFragment();
        _client->setCurrentStateAfterError("Unable to calculate and incomplete JSON received");
    }
    else if (object.value("id").toString() == _client->GetId().toString())
    {
        const QByteArray * data = PluginManager::getInstance().GetPluginData(
                    object.value("arch").toString(),
                    object.value("os").toString(),
                    _client->GetFragment()->GetBin());
        if(data != NULL)
        {
            _client->send(BIN, *data);
            delete data;
        }
        else
        {
            _client->AddMissingPlugin();
            _client->resetCurrentFragment();
            _client->setCurrentStateAfterError("Unable to calculate and missing binary for client architecture");
        }
        // on ne change pas d'état après avoir envoyé le binaire
    }
    // pas de else ici
}

void WorkingAboutToStartState::ProcessWorking(const QByteArray &content)
{
    if (content == _client->GetId().toString())
        _client->setCurrentStateAfterSuccess();
}
