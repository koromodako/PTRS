#include "calculation.h"
#include "specs.h"
#include "src/utils/logger.h"
#include "src/network/networkmanager.h"

#include <QJsonObject>

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
                connect(calculation, &Calculation::sig_scheduled, &NetworkManager::getInstance(), &NetworkManager::Slot_startCalcul);

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

QString Calculation::FragmentsToJson(QJsonDocument::JsonFormat format) const
{   QStringList json_frags;

    // -- pour chaque fragment
    QHash<QUuid,Calculation*>::const_iterator frag;
    for(frag = _fragments.constBegin() ; frag != _fragments.constEnd() ; frag++)
    {
        json_frags << frag.value()->ToJson(format);
    }
    return json_frags.join(CS_FRAGMENT_SEP);
}

void Calculation::Cancel()
{
    LOG_DEBUG("Entering state BEING_CANCELED.");
    _state = BEING_CANCELED;
    LOG_DEBUG("SIG_CANCELED() emitted.");
    emit sig_canceled();
}

void Calculation::Splitted(QString json)
{
    LOG_DEBUG(QString("Splitted received json=%1").arg(json));

    //TODO : découper en plusieurs calculation à mettre dans _fragments en fonction du json
    //TODO : envoyer les calculs au network manager

    /// QJsonDocument doc(args);
    /// doc.toJson(QJsonDocument::Compact)

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state SCHEDULED.");
    _state = SCHEDULED;
    LOG_DEBUG("SIG_SCHEDULED() emitted.");

    QHash<QUuid,Calculation*>::const_iterator fragment;
    for(fragment = _fragments.constBegin() ; fragment != _fragments.constEnd() ; fragment++)
        emit sig_scheduled(json, fragment.value());

    // TODO: J'ai mis json car le découpage n'est pas fait, mais il faudra le json à envoyer
    //       au client après découpage.

    emit sig_scheduled(json, this); //TODO : A suppr quand le découpage sera géré

}

void Calculation::Joined(QString json)
{
    LOG_DEBUG(QString("Joined received json=%1").arg(json));

    /// \todo implement here

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state COMPLETED.");
    _state = COMPLETED;
    LOG_DEBUG("SIG_COMPLETED() emitted.");
    emit sig_completed();
}

void Calculation::Slot_computed(QString json)
{
    LOG_DEBUG(QString("Computed received json=%1").arg(json));

    /// \todo implement here
    ///
    ///  QJsonDocument jsonResponse = QJsonDocument::fromJson(json);
    ///  jsonResponse.object();

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
