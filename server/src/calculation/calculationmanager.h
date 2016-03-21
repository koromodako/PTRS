#ifndef CALCULATIONMANAGER_H
#define CALCULATIONMANAGER_H

#include "calculation.h"

typedef QHash<QUuid, Calculation*> CalculationHash;

/**
 * @brief Cette classe représente l'entité qui gère les calculs distribués commandés au serveur
 */
class CalculationManager : public QObject
{
    Q_OBJECT
public:
    virtual ~CalculationManager(){}

    /**
     * @brief Réalise la fragmentation du calcul et programme son execution
     * @param calculation
     *      Calcul à fragmenter et executer
     * @return false si le binaire de fragmentation correspondant au calcul n'existe pas
     */
    bool Execute(Calculation * calculation);

    /**
     * @brief Démarre le processus d'annulation d'un calcul
     * @param id
     *      Identifiant du calcul à annuler
     * @return false si l'identifiant de calcul n'est pas alloué
     */
    bool Cancel(QUuid id);

    /**
     * @brief Result
     * @return
     */
    QString Result(QUuid id, QString filename) const;

    /**
     * @brief Status
     * @return
     */
    QString Status() const;

    // stats functions

    int ScheduledCount() const;
    int CompletedCount() const;
    int CanceledCount() const;
    int CrashedCount() const;
    inline int Count() const { return _calculations.count(); }

    int AverageLifetime() const;
    int AverageFragmentCount() const;

signals:
    /**
     * @brief Emis quand un calcul est terminé
     * @param idCalculation l'id du calcul
     * @param result le résultat du calcul
     */
    void sig_calculationDone(QUuid idCalculation, const QJsonObject &result);

    /**
     * @brief Emis quand l'avancement d'un calcul est mis à jour
     * @param idCalculation l'id du calcul dont l'avancement a progressé
     * @param value Avancement entre 0 et 100
     */
    void sig_calculationProgressUpdated(QUuid idCalculation, int value);

    /**
     * @brief Emis quand l'état d'un calcul est mis à jour
     * @param idCalculation l'id du calcul dont l'avancement a progressé
     * @param state le nouvel état du calcul
     */
    void sig_calculationStateUpdated(QUuid idCalculation, Calculation::State state);

    /**
     * @brief Emis quand un nouveau calcul doit être effectué
     * @param idCalculation l'id du nouveau calcul
     */
    void sig_newCalculation(QUuid idCalculation);


protected:
    /**
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    static CalculationManager & getInstance();

    // only application manager class can access this instance
    friend class ApplicationManager;

private: // singleton
    CalculationManager(); // interdiction d'instancier en dehors de cette classe

    /**
     * @brief Retourne le nombre de calcul dans l'état donné
     */
    int countWithState(Calculation::State state) const;

    Q_DISABLE_COPY(CalculationManager)   // interdiction de réaliser une copie de l'instance

    CalculationHash _calculations;
};

#endif // CALCULATIONMANAGER_H
