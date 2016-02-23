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
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    static NetworkManager &GetInstance();

public slots:
    /**
     * @brief Trouve un client dispo et demande au client de démarrer le calcul donné
     * @param fragmentId l'id du fragment de calcul (utile pour stopper
     *                   éventuellement le calcul)
     * @param args les arguments de calcul à transmettre au client
     */
    void StartCalcul(int fragmentId, QJsonObject args);

    /**
     * @brief Arrête le calcul avec l'ID donné
     * @param fragmentId l'id du fragment de calcul
     */
    void StopCalcul(int fragmentId);

signals:
    /**
     * @brief Emit quand aucun client ne peut effectuer le calcul
     * @param fragmentId l'id du fragment de calcul
     */
    void calculAborted(int fragmentId);

    /**
     * @brief Emit quand un fragment à fini avec succès son calcul
     * @param fragmentId l'id du fragment de calcul
     * @param args les résultat du calcul transmis par le client
     */
    void calculDone(int fragmentId, QJsonObject args);

private:
    /**
     * @brief Constructeur de la classe
     *        Le constructeur est privé car c'est un singleton
     */
    NetworkManager();

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

private:
    QSet<ClientSession *> _availableClients;
    QMap<int, ClientSession *> _fragmentsPlace;
    TCPServer *_TCPServer;
    UDPServer *_UDPServer;
    QSet<ClientSession *> _unavailableClients;

    Q_DISABLE_COPY(NetworkManager)
};

#endif // NETWORK_MANAGER_H
