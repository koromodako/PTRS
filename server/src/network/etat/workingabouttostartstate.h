#ifndef WORKING_ABOUT_START_STATE_H
#define WORKING_ABOUT_START_STATE_H

#include "abstractstate.h"

/**
 * @brief Cette classe représente l'entité qui gère les calculs distribués commandés au serveur
 */
class WorkingAboutToStartState : public AbstractState
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    WorkingAboutToStartState(ClientSession *parent);

    /**
     * @brief Destructeur de la classe
     */
    virtual ~WorkingAboutToStartState();

    /**
     * @brief Effectue la commande UNABLE
     */
    virtual void ProcessUnable(const QStringList &args) override;

    /**
     * @brief Effectue la commande WORKING
     */
    virtual void ProcessWorking(const QStringList &args) override;
};

#endif // WORKING_ABOUT_START_STATE_H
