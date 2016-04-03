#include "readystate.h"
#include "src/network/clientsession.h"
#include "src/plugins/pluginmanager.h"
#include "src/utils/logger.h"

#include <QJsonObject>
#include <QJsonDocument>

ReadyState::ReadyState(ClientSession *parent) :
    AbstractState(parent),
    _calc_todo(NULL)
{

}

ReadyState::~ReadyState()
{
}

void ReadyState::ProcessDo(const QByteArray &content)
{
    if (content.size() > 0)
    {
        QString error;
        _calc_todo = Calculation::FromJson(_client, content, error);

        if (_calc_todo == NULL)
        {
            LOG_DEBUG(error);
            return;
        }

        //TODO: Vérifier que le calcul est faisable et renvoyer UNABLE s'il ne l'est pas
        if(!PluginManager::getInstance().PluginExists(_calc_todo->GetBin()))
        {
            LOG_DEBUG("Sending UNABLE with id and arch");
            QJsonObject object;
            object.insert("id", _client->Id());
            object.insert("arch", QHOST_ARCH);
            object.insert("os", QHOST_OS);
            _client->Send(UNABLE, QJsonDocument(object).toJson());
            // on ne change pas d'état, c'est le même état qui s'occupe de la reception du plugin
        }
        else
        {
            LOG_DEBUG("Launching calculation on plugin manager");
            _client->Send(WORKING, _client->Id());
            _client->SetCurrentState();
            _client->Slot_startCalculation(_calc_todo);
            _calc_todo = NULL; // this is really dirty but it's a proof of concept work so...
        }
    }
}

void ReadyState::ProcessBin(const QByteArray &content)
{
    if(content.size() > 0 && _calc_todo != NULL)
    {
        if(PluginManager::getInstance().WritePlugin(_calc_todo->GetBin(), content))
        {
            _client->Send(WORKING, _client->Id());
            _client->SetCurrentState();
            _client->Slot_startCalculation(_calc_todo);
        }
        else
        {
            delete _calc_todo;
        }
        _calc_todo = NULL; // this is really dirty but it's a proof of concept work so...
    }
}
