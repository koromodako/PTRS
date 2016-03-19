#ifndef WORKING_STATE_H
#define WORKING_STATE_H

#include "abstractstate.h"

/**
 * @brief Etat en cours de travail
 */
class WorkingState : public AbstractState
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    WorkingState(ClientSession *parent);

    /**
     * @brief Destructeur de la classe
     */
    virtual ~WorkingState();

    /**
     * @brief Effectue la commande ABORT
     */
    virtual void ProcessAbort(const QByteArray &content) override;

    /**
     * @brief Effectue la commande DONE
     */
    virtual void ProcessDone(const QByteArray &content) override;

    /**
     * @brief Effectue la commande STOP
     */
    virtual void ProcessStop() override;
};

#endif // WORKING_STATE_H
