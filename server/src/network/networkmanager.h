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

public slots:

signals:

protected:
    /**
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    NetworkManager & getInstance() { return _instance; }
    // only application manager class can access this instance
    friend class ApplicationManager;

private: // singleton
    NetworkManager();                // interdiction d'instancier en dehors de cette classe
    Q_DISABLE_COPY(NetworkManager)   // interdiction de réaliser une copie de l'instance
    static NetworkManager _instance; // instance unique de la classe

};

#endif // NETWORKMANAGER_H
