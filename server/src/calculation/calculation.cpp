#include "calculation.h"
#include "specs.h"
#include "src/utils/logger.h"
#include "src/network/networkmanager.h"

#include <QJsonArray>

Calculation * Calculation::FromJson(QObject * parent, const QByteArray &json, QString & errorStr)
{
    Calculation * calculation = NULL;

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
            {   calculation = new Calculation(doc.object().value(CS_JSON_KEY_CALC_BIN).toString(),
                                              doc.object().value(CS_JSON_KEY_CALC_PARAMS).toObject().toVariantMap(),
                                              parent);
                connect(calculation, &Calculation::sig_scheduled, &NetworkManager::getInstance(), &NetworkManager::Slot_startCalcul);

            }
        }
        else
        {   errorStr = "Given JSON block is not an object.";
        }
    }
    else
    {   errorStr = error.errorString();
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

QString Calculation::FragmentsResultsToJson(QJsonDocument::JsonFormat format) const
{   QJsonArray array;

    // -- pour chaque fragment
    QHash<QUuid,Calculation*>::const_iterator frag;
    for(frag = _fragments.constBegin() ; frag != _fragments.constEnd() ; frag++)
    {
        array.append(frag.value()->GetResult());
    }
    return QJsonDocument(array).toJson(format);
}

void Calculation::Cancel()
{
    LOG_DEBUG("Entering state BEING_CANCELED.");
    _state = BEING_CANCELED;
    LOG_DEBUG("sig_cancelED() emitted.");
    emit sig_canceled();
}

void Calculation::Splitted(const QByteArray & json)
{
    LOG_DEBUG(QString("Splitted received json=%1").arg(QString(json)));

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {   LOG_ERROR("an error occured while parsing splitted json block.");
        _state = CRASHED;
        return;
    }
    QJsonArray fragments = doc.array();
    foreach (QJsonValue fragment, fragments)
    {
        QString error;
        Calculation * frag = Calculation::FromJson(this, QJsonDocument(fragment.toObject()).toJson(QJsonDocument::Compact), error);
        if(frag != NULL)
        {   _fragments.insert(frag->GetId(), frag);
        }
        else
        {   LOG_ERROR("fragment creation failed !");
            _state = CRASHED;
            return;
        }
    }

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state SCHEDULED.");
    _state = SCHEDULED;
    LOG_DEBUG("SIG_SCHEDULED() emitted.");

    QHash<QUuid,Calculation*>::const_iterator fragment;
    for(fragment = _fragments.constBegin() ; fragment != _fragments.constEnd() ; fragment++)
        emit sig_scheduled(fragment.value()->ToJson(), fragment.value());
}

void Calculation::Joined(const QByteArray &json)
{
    LOG_DEBUG(QString("Joined received json=%1").arg(QString(json)));

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {   LOG_ERROR("an error occured while parsing joined json block.");
        _state = CRASHED;
        return;
    }
    _result = doc.object();

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state COMPLETED.");
    _state = COMPLETED;
    LOG_DEBUG("SIG_COMPLETED() emitted.");
    emit sig_completed();
}

void Calculation::Slot_computed(const QByteArray & json)
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

void Calculation::Slot_crashed(QString error)
{
    LOG_ERROR(QString("Calculation crashed due to the following reason : %1").arg(error.isEmpty() ? "<unknown_reason>" : error));
    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state CRASHED.");
    _state = CRASHED;
    LOG_DEBUG("SIG_CRASHED() emitted.");
    emit sig_crashed();
}

Calculation::Calculation(const QString & bin, const QVariantMap &params, QObject * parent) :
    AbstractIdentifiable(parent),
    _state(BEING_SPLITTED),
    _bin(bin),
    _params(params),
    _fragments()
{
}
