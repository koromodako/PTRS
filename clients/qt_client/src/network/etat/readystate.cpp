#include "readystate.h"
#include "src/network/clientsession.h"
#include "src/plugins/pluginmanager.h"
#include "src/calculation/calculation.h"
#include "src/utils/logger.h"

#include <QJsonObject>
#include <QJsonDocument>

ReadyState::ReadyState(ClientSession *parent) : AbstractState(parent)
{

}

ReadyState::~ReadyState()
{
}

void ReadyState::ProcessDo(const QByteArray &content)
{
    if (content.size() > 0)
    {
        //QJsonDocument jsonResponse = QJsonDocument::fromJson(args[0].toUtf8());
        QString error;
        Calculation * calcToDo = Calculation::FromJson( &PluginManager::getInstance(), content, error);
        //TODO: Vérifier que le calcul est faisable et renvoyer UNABLE s'il ne l'est pas
        LOG_DEBUG("Launching calculation on plugin manager");
        PluginManager::getInstance().Calc(calcToDo);
        //emit _client->sig_requestCalculStart(jsonResponse.object());
        _client->SetCurrentState();

        _client->Send(WORKING);
    }
}
