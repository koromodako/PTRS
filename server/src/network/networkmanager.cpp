#include "networkmanager.h"
#include "src/utils/logger.h"

#include <QThread>

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{

}

int NetworkManager::AvailableClientCount() const
{
    return _availableClients.count();
}

int NetworkManager::ClientCount() const
{
    return _availableClients.count() + _unavailableClients.count();
}

int NetworkManager::WorkingClientCount() const
{
    return _fragmentsPlace.count();
}

NetworkManager &NetworkManager::getInstance()
{
    static NetworkManager instance;
    return instance;
}

void NetworkManager::addAvailableClient(ClientSession *client)
{
    if (client == NULL)
        return;
    if (_unavailableClients.remove(client) && client->Fragment() != NULL)
        _fragmentsPlace.remove(client->Fragment()->GetId());
    LOG_DEBUG("Adding available client");
    _availableClients.insert(client);
}

void NetworkManager::addUnavailableClient(ClientSession *client)
{
    if (client == NULL)
        return;

    bool found = false;
    if (_availableClients.remove(client))
        found = true;
    if (_unavailableClients.remove(client))
    {
        found = true;
        _fragmentsPlace.remove(client->Fragment()->GetId());
    }
    _unavailableClients.insert(client);
    if (!found)
    {
        connect(client, &ClientSession::sig_ready, this, &NetworkManager::addAvailableClient);
        connect(client, &ClientSession::sig_working, this, &NetworkManager::addUnavailableClient);
        connect(client, &ClientSession::sig_disconnected, this, &NetworkManager::slot_deleteClient);

    }
}

void NetworkManager::slot_deleteClient(ClientSession *client)
{
    _availableClients.remove(client);
    if (_unavailableClients.remove(client) && client->Fragment() != NULL)
        _fragmentsPlace.remove(client->Fragment()->GetId());
    client->deleteLater();
}

void NetworkManager::Slot_init()
{
    LOG_INFO("Démarrage du network manager...");

    _TCPServer = new TCPServer(this);
    _UDPServer = new UDPServer(_TCPServer->serverPort(), this);
    connect(_TCPServer, &TCPServer::sig_newConnection, this, &NetworkManager::addUnavailableClient);

    emit sig_started();
}

void NetworkManager::Slot_startCalcul(const QString &json, const Calculation *fragment)
{
    QSet<ClientSession *>::iterator it = _availableClients.begin();
    if (it == _availableClients.end())
    {
        LOG_INFO("Aucun client n'est actuellement disponible pour le calcul.");
        //TODO : Ajouter à une liste de calcul en attente
        return;
    }

    _availableClients.remove(*it);
    _fragmentsPlace.insert(fragment->GetId(), *it);
    _unavailableClients.insert(*it);
    (*it)->StartCalcul(fragment, json);
}
