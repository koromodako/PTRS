#ifndef CALCULATIONORDER_H
#define CALCULATIONORDER_H

#include "src/utils/abstractidentifiable.h"

/**
 * @brief Cette classe représente un bloc de spécification pour un calcul distribuable
 */
class CalculationOrder : public AbstractIdentifiable
{
public:
    ~CalculationOrder(){}

protected:
    CalculationOrder(QObject * parent);
    friend class CalculationFactory; // seule la CalculationFactory est autorisée à créer des instances de cette classe

private:
    Q_DISABLE_COPY(CalculationOrder)
};

#endif // CALCULATIONORDER_H
