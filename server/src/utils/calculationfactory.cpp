#include "../calculation_plugins/common/calculation_specs.h"
#include "calculationfactory.h"

#include <QJsonDocument>
#include <QJsonObject>

Calculation *CalculationFactory::MakeCalculation(QObject *parent, const QByteArray & json)
{
    Calculation * calculation = NULL;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(doc.isObject())
    {   if(doc.object().contains(CS_JSON_KEY_CALC_BIN))
        {   calculation = new Calculation(doc.object().value(CS_JSON_KEY_CALC_BIN).toString(), parent);
        }
    }
    return calculation;
}

CalculationFragment *CalculationFactory::MakeCalculationFragment(QObject *parent, const QByteArray & json)
{
    return new CalculationFragment(parent);
}

