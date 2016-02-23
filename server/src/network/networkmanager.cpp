#include "networkmanager.h"

NetworkManager NetworkManager::_instance;

NetworkManager::NetworkManager()
{
    _TCPServer = new TCPServer(this);
    _UDPServer = new UDPServer(_TCPServer->serverPort(), this);
    connect(_TCPServer, &TCPServer::newConnection, this, &NetworkManager::addUnavailableClient);
}

NetworkManager::~NetworkManager()
{

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
        _fragmentsPlace.remove(client->FragmentId());
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
        _fragmentsPlace.remove(client->FragmentId());
    }
    _unavailableClients.insert(client);
    if (!found)
    {
        connect(client, &ClientSession::ready, this, &NetworkManager::addAvailableClient);
        connect(client, &ClientSession::working, this, &NetworkManager::addUnavailableClient);
    }
}

void NetworkManager::StartCalcul(int fragmentId, QJsonObject args)
{
    QSet<ClientSession *>::iterator it = _availableClients.begin();
    if (it == _availableClients.end())
    {
        emit calculAborted(fragmentId);
        return;
    }

    _availableClients.remove(*it);
    _fragmentsPlace.insert(fragmentId, *it);
    _unavailableClients.insert(*it);
    (*it)->StartCalcul(fragmentId, args);
}

void NetworkManager::StopCalcul(int fragmentId)
{
    ClientSession *client = _fragmentsPlace[fragmentId];
    if (client != NULL)
        client->StopCalcul();
}
