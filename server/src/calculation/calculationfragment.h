#ifndef CALCULATIONFRAGMENT_H
#define CALCULATIONFRAGMENT_H

#include "src/utils/abstractidentifiable.h"

/**
 * @brief Cette classe représente un fragment d'un calcul distribuable
 */
class CalculationFragment : public AbstractIdentifiable
{
public:
    ~CalculationFragment(){}

protected:
    CalculationFragment(QObject * parent = NULL);
    friend class CalculationFactory; // seule la CalculationFactory est autorisée à créer des instances de cette classe

private:
    Q_DISABLE_COPY(CalculationFragment)
};

#endif // CALCULATIONFRAGMENT_H
