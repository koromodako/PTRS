#ifndef CALCULATIONMANAGER_H
#define CALCULATIONMANAGER_H

#include "calculation.h"

/**
 * @brief Cette classe représente l'entité qui gère les calculs distribués commandés au serveur
 */
class CalculationManager
{
public:
    virtual ~CalculationManager(){}

protected:
    /**
     * @brief Récupère l'instance unique correspondant à cette classe
     * @return
     */
    CalculationManager & getInstance() { return _instance; }
    // only application manager class can access this instance
    friend class ApplicationManager;

private: // singleton
    CalculationManager();                // interdiction d'instancier en dehors de cette classe
    Q_DISABLE_COPY(CalculationManager)   // interdiction de réaliser une copie de l'instance
    static CalculationManager _instance; // instance unique de la classe

};

#endif // CALCULATIONMANAGER_H
