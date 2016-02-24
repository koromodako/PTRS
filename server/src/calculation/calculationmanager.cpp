#include "calculationmanager.h"
#include "specs.h"
#include "src/plugins/pluginmanager.h"
#include "src/utils/logger.h"
#include <QJsonObject>
#include <QJsonDocument>

CalculationManager CalculationManager::_instance;

bool CalculationManager::Execute(Calculation *calculation)
{
    bool ok = false;
    // -- on ajoute le calcul à la liste des calculs
    _calculations.insert(calculation->GetId(), calculation);
    // -- si le plugin existe
    if(PluginManager::getInstance().PluginExists(calculation->GetBin()))
    {   // -- démarrer la procédure de fragmentation
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
    /// \todo implement CalculationManager::Result(QUuid id, QString filename)
    ///     Utiliser QProcess pour faire appel aux plugins
    return QString("We are working on this functionality.");
}

QString CalculationManager::Status() const
{
    /// \todo implement CalculationManager::Status()
    ///     Construire la table des status des calculs en cours
    return QString("We are working on this functionality.");
}

int CalculationManager::ScheduledCount() const
{
    int count(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   if(c.value()->GetStatus() == Calculation::SCHEDULED) { count++; }
    }
    return count;
}

int CalculationManager::CompletedCount() const
{
    int count(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   if(c.value()->GetStatus() == Calculation::COMPLETED) { count++; }
    }
    return count;
}

int CalculationManager::CanceledCount() const
{
    int count(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   if(c.value()->GetStatus() == Calculation::CANCELED) { count++; }
    }
    return count;
}

int CalculationManager::CrashedCount() const
{
    int count(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   if(c.value()->GetStatus() == Calculation::CRASHED) { count++; }
    }
    return count;
}

int CalculationManager::AverageLifetime() const
{
    /// \todo implement CalculationManager::AverageLifetime()
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
