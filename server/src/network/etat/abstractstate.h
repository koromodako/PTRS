#ifndef ABSTRACT_STATE_H
#define ABSTRACT_STATE_H

#include <QObject>
#include "src/const.h"

/**
 * @brief Classe abstraite des états
 */
class ClientSession;
class AbstractState : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    AbstractState(ClientSession *parent);

    /**
     * @brief Destructeur de la classe
     */
    virtual ~AbstractState() = 0;

    /**
     * @brief Appelé au moment où l'état est activé, elle
     *        peut être utilisé pour effectuer certaines initialisations
     */
    virtual void OnEntry();

    /**
     * @brief Appelé juste avant que l'état soit desactivé, elle
     *        peut être utilisé pour effectuer certains nettoyage
     */
    virtual void OnExit();

    /**
     * @brief Effectue la commande ABORT
     */
    virtual void ProcessAbort(const QStringList &args);

    /**
     * @brief Effectue la commande ABORT
     */
    virtual void ProcessDo(const QJsonObject &args);

    /**
     * @brief Effectue la commande DONE
     */
    virtual void ProcessDone(const QStringList &args);

    /**
     * @brief Effectue la commande HELLO
     */
    virtual void ProcessHello();

    /**
     * @brief Effectue la commande READY
     */
    virtual void ProcessReady(const QStringList &args);

    /**
     * @brief Effectue la commande STOP
     */
    virtual void ProcessStop();

    /**
     * @brief Effectue la commande UNABLE
     */
    virtual void ProcessUnable(const QStringList &args);

    /**
     * @brief Effectue la commande WORKING
     */
    virtual void ProcessWorking(const QStringList &);

protected:
    ClientSession *_client;
};

#endif // ABSTRACT_STATE_H
