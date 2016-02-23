#include "calculation.h"
#include "specs.h"

#include <QJsonObject>

void Calculation::AddFragment(Calculation *fragment)
{   _fragments.insert(fragment->GetId(), fragment);
}

void Calculation::AddFragment(QList<Calculation*> fragments)
{   foreach (Calculation * calc_frag, fragments) {
        AddFragment(calc_frag);
    }
}

Calculation * Calculation::FromJson(QObject * parent, const QByteArray &json, QString & error_str)
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

QString Calculation::ToJson(QJsonDocument::JsonFormat format) const
{
    QJsonObject calc;
    calc.insert(CS_JSON_KEY_CALC_BIN, GetBin());
    calc.insert(CS_JSON_KEY_CALC_PARAMS, QJsonObject::fromVariantMap(_params));
    QJsonDocument doc(calc);
    return doc.toJson(format);
}

#ifdef QT_DEBUG
QString Calculation::dump() const
{   return QString("\n--------------- CALCULATION_DUMP ---------------\n"
                   "ID : %1\n"
                   "BIN : %2\n"
                   "STATUS : %3\n"
                   "PARAMS : %4\n"
                   "FRAGMENT_COUNT : %5\n"
                   "------------------------------------------------")
            .arg(GetId().toString(), GetBin(),
                 QString::number(GetStatus()),
                 QJsonDocument(QJsonObject::fromVariantMap(GetParams())).toJson(QJsonDocument::Compact),
                 QString::number(GetFragmentCount()));
}
#endif

Calculation::Calculation(const QString & bin, const QVariantMap &params, QObject * parent) :
    AbstractIdentifiable(parent),
    _status(CALCS_SCHEDULED),
    _bin(bin),
    _params(params),
    _fragments()
{
}
