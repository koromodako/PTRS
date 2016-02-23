#include "../calculation_plugins/common/calculation_specs.h"
#include "calculationfactory.h"

#include <QJsonDocument>
#include <QJsonObject>

Calculation *CalculationFactory::MakeCalculation(QObject *parent, const QByteArray & json, QString & error_str)
{
    Calculation * calculation = NULL;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(!error.error)
    {   if(doc.isObject())
        {   bool ok = true;
            if(!doc.object().contains(CS_JSON_KEY_CALC_BIN))
            {   error_str = QString("Missing '%1' key in JSON structure.").arg(CS_JSON_KEY_CALC_BIN);
                ok = false;
            }
            else if(!doc.object().contains(CS_JSON_KEY_CALC_PARAMS) || !doc.object().value(CS_JSON_KEY_CALC_PARAMS).isObject())
            {   error_str = QString("Missing '%1' key in JSON structure or value is not an object.").arg(CS_JSON_KEY_CALC_PARAMS);
                ok = false;
            }
            if(ok)
            {   calculation = new Calculation(doc.object().value(CS_JSON_KEY_CALC_BIN).toString(),
                                              doc.object().value(CS_JSON_KEY_CALC_PARAMS).toObject().toVariantMap(),
                                              parent);
            }
        }
        else
        {   error_str = "Given JSON block is not an object.";
        }
    }
    else
    {   error_str = error.errorString();
    }
    return calculation;
}

CalculationFragment *CalculationFactory::MakeCalculationFragment(QObject *parent, const QByteArray & json, QString & error_str)
{
    return new CalculationFragment(parent);
}

