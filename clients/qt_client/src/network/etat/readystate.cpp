#include "readystate.h"
#include "src/network/clientsession.h"

#include <QJsonObject>
#include <QJsonDocument>

ReadyState::ReadyState(ClientSession *parent) : AbstractState(parent)
{

}

ReadyState::~ReadyState()
{
}

void ReadyState::ProcessDo(const QStringList &args)
{
    if (args.size() > 0)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(args[0].toUtf8());

        //TODO: Vérifier que le calcul est faisable est renvoyer UNABLE s'il ne l'ait pas
        emit _client->sig_requestCalculStart(jsonResponse.object());
        _client->SetCurrentState();

        _client->SendCmd(WORKING, "");
    }
}
