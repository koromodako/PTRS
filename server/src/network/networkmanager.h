#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QMap>
#include <QJsonObject>
#include "src/network/etat/abstractstate.h"
#include "src/network/clientsession.h"
#include "src/const.h"
#include "src/network/tcpserver.h"
#include "src/network/udpserver.h"


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
     * @brief Retourne le nombre de client prêt à travailler
     */
    int AvailableClientCount() const;

    /**
     * @brief Retourne le nombre de client total
     */
    int ClientCount() const;

    /**
     * @brief Retourne le nombre de client en train de travailler
     */
    int WorkingClientCount() const;

public slots:
    /**
     * Initialise le manager et démarre les serveurs UDP et TCP
     */
    void Slot_init();

    /**
     * @brief Trouve un client dispo et demande au client de démarrer le calcul donné
     * @param fragmentId l'id du fragment de calcul (utile pour stopper
     *                   éventuellement le calcul)
     * @param args les arguments de calcul à transmettre au client
     */
    void Slot_startCalcul(const Calculation *fragment);

signals:
    /**
     * Emis quand le network manager et les serveurs ont démarré
     */
    void sig_started();

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
    /**
     * @brief Ajoute le client à la liste des clients disponibles
     *        Si le client était dans une autre liste, elle le supprime
     *        de son ancienne liste
     * @param client le client à déplacer
     */
    void addAvailableClient(ClientSession *client);

    /**
     * @brief Ajoute le client à la liste des clients indisponibles
     *        Si le client était dans une autre liste, elle le supprime
     *        de son ancienne liste
     * @param client le client à ajouter/déplacer
     */
    void addUnavailableClient(ClientSession *client);

    /**
     * @brief Supprime la session avec le client donné
     */
    void slot_deleteClient(ClientSession *client);

private:
    QSet<ClientSession *> _availableClients;
    QMap<QUuid, ClientSession *> _fragmentsPlace;
    TCPServer *_TCPServer;
    UDPServer *_UDPServer;
    QSet<ClientSession *> _unavailableClients;

    Q_DISABLE_COPY(NetworkManager)
};

#endif // NETWORK_MANAGER_H
