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

void NetworkManager::slot_addAvailableClient(ClientSession *client)
{
    if (client == NULL)
        return;
    if (_unavailableClients.remove(client) && client->GetFragment() != NULL)
    {
        _fragmentsPlace.remove(client->GetFragment()->GetId());
        emit sig_workingClientCountUpdated(_fragmentsPlace.count());
    }

    emit sig_availableClientCountUpdated(_availableClients.count());

    LOG_DEBUG("Adding available client");
    _availableClients.insert(client);
    if (_waitingFragments.size() > 0)
    {
        Slot_startCalcul(_waitingFragments.dequeue());
        emit sig_waitingCalculationCountUpdated(_waitingFragments.count())
    }
}

void NetworkManager::slot_addUnavailableClient(ClientSession *client)
{
    if (client == NULL)
        return;

    bool found = false;
    if (_availableClients.remove(client))
        found = true;
    if (_unavailableClients.remove(client))
    {
        found = true;
        _fragmentsPlace.remove(client->GetFragment()->GetId());
        emit sig_workingClientCountUpdated(_fragmentsPlace.count());
    }
    _unavailableClients.insert(client);
    if (!found)
    {
        emit sig_clientCountUpdated(ClientCount());
        connect(client, &ClientSession::sig_unableToCalculate, this, &NetworkManager::Slot_startCalcul);
        connect(client, &ClientSession::sig_ready, this, &NetworkManager::slot_addAvailableClient);
        connect(client, &ClientSession::sig_working, this, &NetworkManager::slot_addUnavailableClient);
        connect(client, &ClientSession::sig_disconnected, this, &NetworkManager::slot_deleteClient);
    }

    emit sig_availableClientCountUpdated(_availableClients.count());
}

void NetworkManager::slot_deleteClient(ClientSession *client)
{
    _availableClients.remove(client);
    if (_unavailableClients.remove(client) && client->GetFragment() != NULL)
    {
        _fragmentsPlace.remove(client->GetFragment()->GetId());
        emit sig_workingClientCountUpdated(_fragmentsPlace.count());
    }
    client->deleteLater();

    emit sig_clientCountUpdated(ClientCount());
    emit sig_availableClientCountUpdated(_availableClients.count());
}

void NetworkManager::Slot_init()
{
    LOG_INFO("Démarrage du network manager...");

    _TCPServer = new TCPServer(this);
    _UDPServer = new UDPServer(_TCPServer->serverPort(), this);
    connect(_TCPServer, &TCPServer::sig_newConnection, this, &NetworkManager::slot_addUnavailableClient);

    emit sig_started();
}

void NetworkManager::Slot_startCalcul(const Calculation *fragment)
{
    QSet<ClientSession *>::iterator it = _availableClients.begin();
    bool add = false;
    if (it == _availableClients.end())
        add = true;
    else
    {
        _availableClients.remove(*it);
        _fragmentsPlace.insert(fragment->GetId(), *it);
        _unavailableClients.insert(*it);
        add = !(*it)->StartCalcul(fragment);

        emit sig_workingClientCountUpdated(_fragmentsPlace.count());
        emit sig_availableClientCountUpdated(_availableClients.count());
    }

    if (add)
    {
        LOG_INFO("Mise en attente du calcul.");
        _waitingFragments.enqueue(fragment);
        emit sig_waitingCalculationCountUpdated(_waitingFragments.count())
    }
}
