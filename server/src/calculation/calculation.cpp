#include "calculation.h"
#include "specs.h"
#include "src/utils/logger.h"
#include "src/network/networkmanager.h"
#include "src/plugins/pluginmanager.h"

#include <QJsonArray>

QString Calculation::StatusToString(Calculation::Status status)
{
    switch (status) {
    case BEING_SPLITTED:    return "BEING_SPLITTED";
    case SCHEDULED:         return "SCHEDULED";
    case BEING_COMPUTED:    return "BEING_COMPUTED";
    case BEING_JOINED:      return "BEING_JOINED";
    case COMPLETED:         return "COMPLETED";
    case CANCELED:          return "CANCELED";
    case CRASHED:           return "CRASHED";
    }
    return "Unhandled state";
}

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
    calc.insert(CS_JSON_KEY_FRAG_ID, GetId().toString());
    calc.insert(CS_JSON_KEY_CALC_PARAMS, QJsonObject::fromVariantMap(_params));
    QJsonDocument doc(calc);
    return doc.toJson(format);
}

QString Calculation::FragmentsResultsToJson(QJsonDocument::JsonFormat format) const
{   QJsonArray array;

    foreach(Fragment *c, _fragments)// -- pour chaque fragment
        array.append(c->GetResult());

    return QJsonDocument(array).toJson(format);
}

void Calculation::Cancel()
{
    LOG_DEBUG("Entering state CANCELED.");
    setCurrentStatus(CANCELED);
    updateProgress(100);

    LOG_DEBUG("sig_canceled() emitted for all children.");
    QHash<QUuid,Fragment*>::const_iterator fragment;
    for(fragment = _fragments.constBegin() ; fragment != _fragments.constEnd() ; fragment++)
        emit fragment.value()->sig_canceled();
}

void Calculation::Splitted(const QByteArray & json)
{
    LOG_DEBUG(QString("Splitted received json=%1").arg(QString(json)));

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {   LOG_ERROR("an error occured while parsing splitted json block.");
        setCurrentStatus(CRASHED);
        return;
    }
    QJsonArray fragments = doc.array();
    foreach (QJsonValue fragment, fragments)
    {
        QString error;
        Fragment * frag = Fragment::FromJson(this, QJsonDocument(fragment.toObject()).toJson(QJsonDocument::Compact), error);
        if(frag != NULL)
        {   _fragments.insert(frag->GetId(), frag);
            connect(frag, &Fragment::sig_progressUpdated, this, &Calculation::slot_updateChildrenProgress);
        }
        else
        {   LOG_ERROR("fragment creation failed !");
            setCurrentStatus(CRASHED);
            return;
        }
    }

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state SCHEDULED.");
    setCurrentStatus(SCHEDULED);
    LOG_DEBUG("SIG_SCHEDULED() emitted.");

    QHash<QUuid,Fragment*>::const_iterator fragment;
    for(fragment = _fragments.constBegin() ; fragment != _fragments.constEnd() ; fragment++)
        emit sig_scheduled(fragment.value());
}

void Calculation::Joined(const QByteArray &json)
{
    LOG_DEBUG(QString("Joined received json=%1").arg(QString(json)));

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {   LOG_ERROR("an error occured while parsing joined json block.");
        setCurrentStatus(CRASHED);
        updateProgress(0);
        return;
    }
    _result = doc.object();

    //Nettoyage des fragments
    qDeleteAll(_fragments);
    _fragments.clear();

    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state COMPLETED.");
    setCurrentStatus(COMPLETED);
    LOG_DEBUG("sig_calculationDone() Emited");
    emit sig_calculationDone(GetId(), GetResult());
}

void Calculation::Crashed(QString error)
{
    LOG_ERROR(QString("Calculation crashed due to the following reason : %1").arg(error.isEmpty() ? "<unknown_reason>" : error));
    // mise à jour de l'état du calcul
    LOG_DEBUG("Entering state CRASHED.");
    updateProgress(0);
    setCurrentStatus(CRASHED);
}

void Calculation::Slot_started()
{
    if (_status == SCHEDULED)
    {
        LOG_DEBUG("Entering state BEING COMPUTED.");
        setCurrentStatus(BEING_COMPUTED);
    }
}

void Calculation::setCurrentStatus(Calculation::Status status)
{
    _status = status;
    emit sig_stateUpdated(GetId(), _status);
}

Calculation::Calculation(const QString & bin, const QVariantMap &params, QObject * parent) :
    AbstractIdentifiable(parent),
    _status(BEING_SPLITTED),
    _bin(bin),
    _params(params),
    _fragments(),
    _progress(0)
{
}

void Calculation::slot_updateChildrenProgress(int oldChildProgress, int newChildProgress)
{
    updateProgress(_progress - oldChildProgress + newChildProgress);

    if (_progress/_fragments.size() >= 100)//Si tous les fragments on terminé le calcul...
    {
        LOG_DEBUG("Entering state BEING_JOINED.");
        setCurrentStatus(BEING_JOINED);
        PluginManager::getInstance().Join(this);
    }
}

void Calculation::updateProgress(int progress)
{
    if(!_fragments.isEmpty())
    {
        LOG_DEBUG("New progress for " + GetId().toString() + " : " + QString::number(progress/_fragments.size()));
        _progress = progress;
        emit sig_progressUpdated(GetId(), _progress/_fragments.size());
    }
}
