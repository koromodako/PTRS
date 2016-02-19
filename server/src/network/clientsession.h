#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QThread>

/**
 * @brief Cette classe représente une session client, c'est à dire une connexion client active.
 *      Les instances s'executent dans des threads séparés de la boucle d'évènement principale de Qt.
 */
class ClientSession : public QThread
{
public:
    ~ClientSession(){}
    ClientSession();

protected:
    void run();

};

#endif // CLIENTSESSION_H
