#ifndef READY_STATE_H
#define READY_STATE_H

#include "abstractstate.h"

/**
 * @brief Etat prêt
 */
class ReadyState : public AbstractState
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    ReadyState(ClientSession *parent);

    /**
     * @brief Destructeur de la classe
     */
    virtual ~ReadyState();

    /**
     * @brief Emet le signal ClientSession::ready.
     */
    virtual void OnEntry() override;

    /**
     * @brief Emet le signal ClientSession::working.
     */
    virtual void OnExit() override;

    /**
     * @brief Effectue la commande DO
     */
    void ProcessDo(const QString &args) override;
};

#endif // READY_STATE_H
