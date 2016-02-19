#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

/**
 * @brief Cette classe est chargée du serveur d'écoute qui crée des connexions avec les clients qui en font la demande
 * @see ClientSession
 */
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    ~NetworkManager(){}
    /**
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    NetworkManager & GetInstance() { return _instance; }

public slots:

signals:


private: // singleton
    NetworkManager();                // interdiction d'instancier en dehors de cette classe
    Q_DISABLE_COPY(NetworkManager)   // interdiction de réaliser une copie de l'instance
    static NetworkManager _instance; // instance unique de la classe

};

#endif // NETWORKMANAGER_H
