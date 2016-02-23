#ifndef DISCONNECTED_STATE_H
#define DISCONNECTED_STATE_H

#include "abstractstate.h"

/**
 * @brief Etat deconnecté
 */
class DisconnectedState : public AbstractState
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    DisconnectedState(ClientSession *parent);

    /**
     * @brief Destructeur de la classe
     */
    virtual ~DisconnectedState();

    /**
     * @brief Effectue la commande HELLO
     */
    virtual void ProcessHello() override;
};

#endif // DISCONNECTED_STATE_H
