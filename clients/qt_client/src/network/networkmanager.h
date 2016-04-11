#ifndef NETWORKMANAGER_H
#include <QObject>

#include "clientsession.h"
/**
 * @brief Cette classe est chargée du serveur d'écoute qui crée des connexions avec les clients qui en font la demande
 * @see ClientSession
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Destructeur de la classe
     */
    ~NetworkManager();

    /**
     * @brief Retourne la ClientSession actuelle
     * @return
     */
    inline ClientSession* GetClientSession() { return _clientSession;}

public slots:
    /**
     * Initialise le manager et démarre les serveurs UDP et TCP
     */
    void Slot_init();



private:
    /**
     * @brief Constructeur de la classe
     *        Le constructeur est privé car c'est un singleton
     */
    NetworkManager();
    /**
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    static NetworkManager &getInstance();
    friend class ApplicationManager;
    friend class Calculation;

private slots:


private:
    ClientSession*_clientSession;

    Q_DISABLE_COPY(NetworkManager);
};

#endif // NETWORK_MANAGER_H
