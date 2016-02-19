#ifndef CALCULATIONMANAGER_H
#define CALCULATIONMANAGER_H

#include <QObject>
#include "src/console/consolehandler.h"
#include "src/calculation/calculationorder.h"

/**
 * @brief Cette classe représente l'entité qui gère les calculs distribués commandés au serveur
 */
class CalculationManager : public QObject
{
    Q_OBJECT

public:
    ~CalculationManager(){}
    /**
     * @brief Récupère l'instance unique de cette classe
     * @return
     */
    static CalculationManager & GetInstance() { return _instance; }

public slots:
    /**
     * @brief Ce slot reçoit les demandes d'exécution de calculs
     * @param calculationOrder
     *      Objet contenant les spécification du calcul à effectuer
     */
    void SLOT_EXEC(CalculationOrder & calculationOrder);
    /**
     * @brief Ce slot reçoit les demandes de status des calculs en cours
     */
    void SLOT_STATUS();
    /**
     * @brief Ce slot reçoit les demandes de résultats pour un calcul en particulier
     * @param id
     *      Identifiant du calcul dont on doit retourner le résultat
     */
    void SLOT_RESULT(uint id);
    /**
     * @brief Ce slot reçoit les demandes d'annulation pour un calcul en particulier
     * @param id
     *      Identifiant du calcul que l'on doit annuler
     */
    void SLOT_CANCEL(uint id);

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


private: // singleton
    CalculationManager();                // interdiction d'instancier en dehors de cette classe
    Q_DISABLE_COPY(CalculationManager)   // interdiction de réaliser une copie de l'instance
    static CalculationManager _instance; // instance unique de la classe

};

#endif // CALCULATIONMANAGER_H
