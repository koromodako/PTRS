#include "networkmanager.h"
#include "src/utils/logger.h"

#include <QThread>

NetworkManager::NetworkManager(){

}

NetworkManager::~NetworkManager()
{

}

NetworkManager &NetworkManager::getInstance()
{
    static NetworkManager instance;
    return instance;
}


void NetworkManager::Slot_init()
{
    LOG_INFO("Démarrage du network manager...");
    _clientSession = NULL;
    //emit sig_started();
}

