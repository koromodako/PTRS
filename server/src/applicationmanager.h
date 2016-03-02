#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QThread>

#include "src/const.h"
#include "calculation/calculationmanager.h"
#include "network/networkmanager.h"

/*
 *  Cette valeur définit le nombre de module dont l'arrêt doit être validé avant
 *  de pouvoir terminer l'application. Pour l'instant on doit attendre la fin
 *  confirmée du PluginManager et du ConsoleHandler avant de terminer.
 */
#define TERMINATED_EXPECTED_TOTAL 2

/**
 * @brief Ce manager gère fait office de facade pour tous les autres managers.
 *      Son rôle est de garantir la cohérence globale de l'application.
 */
class ApplicationManager : public QObject
{
    Q_OBJECT
public:
    ~ApplicationManager();
    static ApplicationManager & GetInstance() { return _instance; }

    void Init();

public slots:
    /**
     * @brief Ce slot reçoit les demandes de consultation de l'état du serveur
     */
    void SLOT_STATE();
    /**
     * @brief Ce slot reçoit les demandes d'exécution de calculs
     * @param calculationOrder
     *      Objet contenant les spécification du calcul à effectuer
     */
    void SLOT_EXEC(QByteArray json);
    /**
     * @brief Ce slot reçoit les demandes de status des calculs en cours
     */
    void SLOT_STATUS();
    /**
     * @brief Ce slot reçoit les demandes de résultats pour un calcul en particulier
     * @param id
     *      Identifiant du calcul dont on doit retourner le résultat
     */
    void SLOT_RESULT(QUuid id, QString filename);
    /**
     * @brief Ce slot reçoit les demandes d'annulation pour un calcul en particulier
     * @param id
     *      Identifiant du calcul que l'on doit annuler
     */
    void SLOT_CANCEL(QUuid id);
    /**
     * @brief Ce slot reçoit les demandes d'arrêt du serveur
     */
    void SLOT_SHUTDOWN();
    /**
     * @brief Ce slot reçoit les notifications d'arrêt des composants
     */
    void SLOT_TERMINATED();

signals:
    /**
     * @brief Ce signal est émis en réponse aux demandes reçues par les slots.
     * @param command
     *      Commande à l'origine de l'émission de cette réponse
     * @param ok
     *      Statut de la réponse, 'true' si la demande à pu être traitée correctement
     * @param message
     *      Message détaillant la réponse
     */
    void SIG_RESPONSE(Command command, bool ok, QString message);
    /**
     * @brief Ce signal est émis une fois que tous les composants se sont arrêtés correctement.
     */
    void SIG_TERMINATE();
    /**
     * @brief Ce signal est émis dès que SLOT_SHUTDOWN() est appelé
     */
    void SIG_TERMINATE_MODULE();

private:
    ApplicationManager();
    Q_DISABLE_COPY(ApplicationManager)
    static ApplicationManager _instance;
    QThread _networkThread;

    int _terminated_ctr;
};

#endif // APPLICATIONMANAGER_H
