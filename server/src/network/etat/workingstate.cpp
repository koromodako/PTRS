#include "workingstate.h"
#include "src/network/clientsession.h"
#include "src/network/networkmanager.h"
#include "src/utils/logger.h"

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
        _client->resetCurrentFragment();
        _client->setCurrentStateAfterError("CalculAborted");
    }
}

void WorkingState::ProcessDone(const QByteArray &content)
{
    if (content.startsWith(_client->GetId().toString().toUtf8()))
    {
        LOG_DEBUG(QString("Computed received json=%1").arg(QString(content.mid(_client->GetId().toString().size()))));

        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(content.mid(_client->GetId().toString().size()), &jsonError);
        if(jsonError.error != QJsonParseError::NoError)
        {
            _client->resetCurrentFragment();
            _client->setCurrentStateAfterError("An error occured while parsing fragment result json block.");
        }
        else
        {
            emit _client->sig_calculDone(doc.object());
            _client->resetCurrentFragment();
            _client->setCurrentStateAfterSuccess();
        }
    }
}

void WorkingState::ProcessStop()
{
    _client->send(STOP);
    _client->setCurrentStateAfterSuccess();
}
