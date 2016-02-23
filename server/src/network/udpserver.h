#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QUdpSocket>

/**
 * @brief Cette classe représente le serveur UDP qui recoit les requêtes broadcastées des clients
 * pour obtenir l'IP et le port du serveur
 */
class UDPServer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    UDPServer(quint16 tcpServerPort, QObject *parent);

    /**
     * @brief Destructeur de la classe
     */
    ~UDPServer();

private slots:
    /**
     * @brief Reçoit les messages UDP qui sont broadcastés et transmet l'ip et le port du server TCP
     */
    void readBroadcastDatagram();

private:
    QUdpSocket _broadcastSocket;
    quint16 _tcpServerPort;
};

#endif
