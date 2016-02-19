#include "calculationfactory.h"

Calculation *CalculationFactory::MakeCalculation(QObject *parent)
{
    return new Calculation(parent);
}

CalculationOrder *CalculationFactory::MakeCalculationOrder(QObject *parent)
{
    return new CalculationOrder(parent);
}

CalculationFragment *CalculationFactory::MakeCalculationFragment(QObject *parent)
{
    return new CalculationFragment(parent);
}

