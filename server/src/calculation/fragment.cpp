#include "fragment.h"

#include <QJsonDocument>
#include <QJsonParseError>

#include "specs.h"
#include "src/network/networkmanager.h"
#include "../utils/logger.h"

Fragment::Fragment(const QString &bin, const QVariantMap &params, QObject * parent) :
    AbstractIdentifiable(parent),
    _bin(bin),
    _params(params),
    _state(BEING_COMPUTED)
{
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

Fragment * Fragment::FromJson(QObject * parent, const QByteArray &json, QString & errorStr)
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
                connect(fragment, &Fragment::sig_scheduled, &NetworkManager::getInstance(), &NetworkManager::Slot_startCalcul);

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

void Fragment::Slot_computed(const QByteArray & json)
{
    LOG_DEBUG(QString("Computed received json=%1").arg(QString(json)));

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {   LOG_ERROR("an error occured while parsing fragment result json block.");
        _state = CRASHED;
        return;
    }
    _result = doc.object();

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state COMPUTED.");
    _state = COMPUTED;
    LOG_DEBUG("SIG_COMPUTED() emitted.");
    emit sig_computed();
}

void Fragment::Slot_crashed(QString error)
{
    LOG_ERROR(QString("Fragment crashed due to the following reason : %1").arg(error.isEmpty() ? "<unknown_reason>" : error));
    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state CRASHED.");
    _state = CRASHED;
    LOG_DEBUG("SIG_CRASHED() emitted.");
    emit sig_crashed();
}
