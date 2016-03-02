#include "networkmanager.h"
#include "src/utils/logger.h"

NetworkManager NetworkManager::_instance;

NetworkManager::NetworkManager()
{
    _TCPServer = new TCPServer(this);
    _UDPServer = new UDPServer(_TCPServer->serverPort(), this);
    connect(_TCPServer, &TCPServer::sig_newConnection, this, &NetworkManager::addUnavailableClient);
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
    return _unavailableClients.count();
}

NetworkManager &NetworkManager::getInstance()
{
    return _instance;
}

void NetworkManager::addAvailableClient(ClientSession *client)
{
    if (client == NULL)
        return;
    if (_unavailableClients.remove(client))
        _fragmentsPlace.remove(client->Fragment()->GetId());
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
    }
}

void NetworkManager::Slot_startCalcul(const QString &json, const Calculation *fragment)
{
    QSet<ClientSession *>::iterator it = _availableClients.begin();
    if (it == _availableClients.end())
    {
        LOG_INFO("Aucun client n'est disponible pour le calcul.");
        //TODO : Ajouter à une liste de calcul en attente
        return;
    }

    _availableClients.remove(*it);
    _fragmentsPlace.insert(fragment->GetId(), *it);
    _unavailableClients.insert(*it);
    (*it)->StartCalcul(fragment, json);
}
