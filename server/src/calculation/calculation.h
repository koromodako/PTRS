#ifndef CALCULATION_H
#define CALCULATION_H

#include "src/utils/abstractidentifiable.h"

/**
 * @brief Cette classe représente un calcul distribuable
 */
class Calculation : public AbstractIdentifiable
{
public:
    ~Calculation(){}

protected:
    Calculation(QObject * parent = NULL);
    friend class CalculationFactory; // seule la CalculationFactory est autorisée à créer des instances de cette classe

private:
    Q_DISABLE_COPY(Calculation)

};

#endif // CALCULATION_H
