#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpServer>
#include "clientsession.h"

/**
 * @brief Cette classe représente le serveur TCP qui recoit les demandes de connexion des clients
 */
class TCPServer : public QTcpServer
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    TCPServer(QObject *parent = 0);

    /**
     * @brief Destructeur de la classe
     */
    ~TCPServer();

signals:
    /**
     * @brief Signal emit quand une nouvelle connection arrive sur le serveur.
     */
    void sig_newConnection(ClientSession *newClient);

protected:
    /**
     * @brief Récupère et créé les connections avec les clients
     */
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif
