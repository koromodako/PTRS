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
        QString error;
        Calculation * calcToDo = Calculation::FromJson(_client, content, error);

        //TODO: Vérifier que le calcul est faisable et renvoyer UNABLE s'il ne l'est pas

        if (calcToDo == NULL)
        {
            LOG_DEBUG(error);
            return;
        }
        LOG_DEBUG("Launching calculation on plugin manager");
        _client->Send(WORKING, _client->Id());
        _client->SetCurrentState();
        _client->Slot_startCalculation(calcToDo);
    }
}
