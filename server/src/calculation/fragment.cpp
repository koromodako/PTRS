#include "fragment.h"

#include <QJsonDocument>
#include <QJsonParseError>

#include "specs.h"
#include "src/network/networkmanager.h"
#include "../utils/logger.h"
#include "calculation.h"

Fragment::Fragment(const QString &bin, const QVariantMap &params, Calculation * parent) :
    AbstractIdentifiable(parent),
    _bin(bin),
    _calculation(parent),
    _params(params),
    _progress(0)
{

}

const Calculation * Fragment::GetCalculation() const
{
    return _calculation;
}

QString Fragment::ToJson(QJsonDocument::JsonFormat format) const
{
    QJsonObject frag;
    frag.insert(CS_JSON_KEY_CALC_BIN, GetBin());
    frag.insert(CS_JSON_KEY_FRAG_ID, GetId().toString());
    frag.insert(CS_JSON_KEY_CALC_PARAMS, QJsonObject::fromVariantMap(_params));
    QJsonDocument doc(frag);
    return doc.toJson(format);
}

Fragment * Fragment::FromJson(Calculation * parent, const QByteArray &json, QString & errorStr)
{
    Fragment * fragment = NULL;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(!error.error)
    {   if(doc.isObject())
        {   bool ok = true;
            if(!doc.object().contains(CS_JSON_KEY_CALC_BIN))
            {   errorStr = QString("Missing '%1' key in JSON structure.").arg(CS_JSON_KEY_CALC_BIN);
                ok = false;
            }
            else if(!doc.object().contains(CS_JSON_KEY_CALC_PARAMS) || !doc.object().value(CS_JSON_KEY_CALC_PARAMS).isObject())
            {   errorStr = QString("Missing '%1' key in JSON structure or value is not an object.").arg(CS_JSON_KEY_CALC_PARAMS);
                ok = false;
            }
            if(ok)
            {   fragment = new Fragment(doc.object().value(CS_JSON_KEY_CALC_BIN).toString(),
                                              doc.object().value(CS_JSON_KEY_CALC_PARAMS).toObject().toVariantMap(),
                                              parent);
            }
        }
        else
        {   errorStr = "Given JSON block is not an object.";
        }
    }
    else
    {   errorStr = error.errorString();
    }
    return fragment;
}

void Fragment::Slot_computed(const QJsonObject &json)
{
    _result = json;

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state COMPUTED.");
    Slot_updateProgress(100);
}

void Fragment::Slot_updateProgress(int progress)
{
    LOG_DEBUG("New progress for fragment " + GetId().toString() + " : " + QString::number(progress));
    emit sig_progressUpdated(_progress, progress);
    _progress = progress % 101;
}
