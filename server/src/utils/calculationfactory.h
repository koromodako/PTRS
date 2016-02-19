#ifndef CALCULATIONFACTORY_H
#define CALCULATIONFACTORY_H

#include "src/calculation/calculation.h"
#include "src/calculation/calculationorder.h"
#include "src/calculation/calculationfragment.h"

/**
 * @brief Cette classe réalise les tâches de création des objets liés aux calculs
 *      Elle seule peut créer ces objets ce qui permet d'assurer le cycle de vie de ces derniers.
 */
class CalculationFactory
{
public:
    /**
     * @brief Crée une nouvelle instance de calcul en lui attachant le parent passé en paramètre
     * @param parent
     *      QObject parent de l'instance qui sera créée
     * @return
     */
    static Calculation * MakeCalculation(QObject * parent);
    /**
     * @brief Crée une nouvelle instance de requete de calcul en lui attachant le parent passé en paramètre
     * @param parent
     *      QObject parent de l'instance qui sera créée
     * @return
     */
    static CalculationOrder * MakeCalculationOrder(QObject * parent);
    /**
     * @brief Crée une nouvelle instance de fragment de calcul en lui attachant le parent passé en paramètre
     * @param parent
     *      QObject parent de l'instance qui sera créée
     * @return
     */
    static CalculationFragment * MakeCalculationFragment(QObject * parent);

private: // classe de type fabrique
    CalculationFactory(){} // non instanciable
    Q_DISABLE_COPY(CalculationFactory) // non copiable
};

#endif // CALCULATIONFACTORY_H
