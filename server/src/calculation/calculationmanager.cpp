#include "calculationmanager.h"
#include "specs.h"
#include "src/plugins/pluginmanager.h"
#include "src/utils/logger.h"
#include <QJsonObject>
#include <QJsonDocument>

bool CalculationManager::Execute(Calculation *calculation, QByteArray params)
{
    bool ok = false;
    // -- on ajoute le calcul à la liste des calculs
    _calculations.insert(calculation->GetId(), calculation);
    // -- si le plugin existe
    if(PluginManager::getInstance().PluginExists(calculation->GetBin()))
    {

        QJsonDocument doc = QJsonDocument::fromJson(params);
        emit sig_newCalculation(calculation->GetId(), doc);
        connect (calculation, SIGNAL(sig_progressUpdated(QUuid, int)),
                 this, SIGNAL(sig_calculationProgressUpdated(QUuid, int)));
        connect (calculation, SIGNAL(sig_stateUpdated(QUuid, Calculation::State)),
                 this, SIGNAL(sig_calculationStateUpdated(QUuid, Calculation::State)));
        connect (calculation, SIGNAL(sig_calculationDone(QUuid, const QJsonObject &)),
                 this, SIGNAL(sig_calculationDone(QUuid, const QJsonObject &)));

        // -- démarrer la procédure de fragmentation
        PluginManager::getInstance().Split(calculation);
        // -- on lève le flag
        ok = true;
    }
    else
    {   // -- on log l'erreur
        LOG_ERROR("Call Execute on a missing plugin !");
        // -- on annule le calcul
        calculation->Crashed("Plugin missing !");
    }
    return ok;
}

bool CalculationManager::Cancel(QUuid id)
{
    bool ok = false;
    CalculationHash::iterator c = _calculations.find(id);
    if(c != _calculations.end())
    {   c.value()->Cancel();
    }
    else
    {   LOG_WARN("Trying to cancel an unknown calculation...");
    }
    return ok;
}

QString CalculationManager::Result(QUuid id, QString filename) const
{
    CalculationHash::const_iterator it = _calculations.find(id);
    if(it != _calculations.end())
    {

        return QJsonDocument(it.value()->GetResult()).toJson();
    }
    return "";
}

QString CalculationManager::Status() const
{
    /// TODO implement CalculationManager::Status()
    ///     Construire la table des status des calculs en cours
    return QString("We are working on this functionality.");
}

int CalculationManager::ScheduledCount() const
{
    return countWithState(Calculation::SCHEDULED);
}

int CalculationManager::CompletedCount() const
{
    return countWithState(Calculation::COMPLETED);
}

int CalculationManager::CanceledCount() const
{
    return countWithState(Calculation::CANCELED);
}

int CalculationManager::CrashedCount() const
{
    return countWithState(Calculation::CRASHED);
}

int CalculationManager::countWithState(Calculation::State state) const
{
    int count(0);
    CalculationHash::const_iterator c;
    for(c = _calculations.constBegin() ; c != _calculations.constEnd() ; ++c)
    {
        if(c.value()->GetStatus() == state)
            count++;
    }
    return count;
}

int CalculationManager::AverageLifetime() const
{
    /// TODO implement CalculationManager::AverageLifetime()
    ///     Ajouter au calcul ce qu'il faut pour évaluer ce temps différence entre date de debut et date de complétion
    return -1;
}

int CalculationManager::AverageFragmentCount() const
{
    int avg(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   avg += c.value()->GetFragmentCount();
    }
    if(_calculations.count() > 0) { avg /= _calculations.count(); }
    return avg;
}

CalculationManager::CalculationManager()
{}

CalculationManager &CalculationManager::getInstance()
{
    static CalculationManager instance;
    return instance;
}

