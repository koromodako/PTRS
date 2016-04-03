#ifndef READY_STATE_H
#define READY_STATE_H

#include "abstractstate.h"
#include "src/calculation/calculation.h"

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
     * @brief Effectue la commande DO
     */
    virtual void ProcessDo(const QByteArray &content) override;

    /**
     * @brief Effectue la commande BIN
     */
    virtual void ProcessBin(const QByteArray &content) override;

private:
    Calculation * _calc_todo;

};

#endif // READY_STATE_H
