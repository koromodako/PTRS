#ifndef CALCULATIONMANAGER_H
#define CALCULATIONMANAGER_H

#include "calculation.h"

/**
 * @brief Cette classe représente l'entité qui gère les calculs distribués commandés au serveur
 */
class CalculationManager : public QObject
{
    Q_OBJECT
public:
    virtual ~CalculationManager(){}

protected:
    /**
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    static CalculationManager & getInstance() { return _instance; }
    // only application manager class can access this instance
    friend class ApplicationManager;

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

private: // singleton
    CalculationManager();                // interdiction d'instancier en dehors de cette classe
    Q_DISABLE_COPY(CalculationManager)   // interdiction de réaliser une copie de l'instance
    static CalculationManager _instance; // instance unique de la classe

    QHash<QUuid, Calculation*> _calculations;

};

#endif // CALCULATIONMANAGER_H
