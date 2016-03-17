#ifndef WAITING_STATE_H
#define WAITING_STATE_H

#include "abstractstate.h"

/**
 * @brief Etat d'attente
 */
class WaitingState : public AbstractState
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    WaitingState(ClientSession *parent);

    /**
     * @brief Destructeur de la classe
     */
    virtual ~WaitingState();

    /**
     * @brief Effectue la commande OK
     */
    virtual void ProcessOK(const QByteArray &content) override;
};

#endif // WAITING_STATE_H
