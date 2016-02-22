#include "calculationmanager.h"

CalculationManager CalculationManager::_instance;

bool CalculationManager::Execute(Calculation *calculation)
{
    /// \todo implement CalculationManager::Execute(Calculation *calculation)
    ///         Utiliser QProcess pour faire appel aux plugins
    return false;
}

bool CalculationManager::Cancel(QUuid id)
{
    /// \todo implement CalculationManager::Cancel(QUuid id)
    ///         Demander au network manager de notifier les clients de l'annulation d'un calcul
    return false;
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
    {   if(c.value()->GetStatus() == CALCS_SCHEDULED) { count++; }
    }
    return count;
}

int CalculationManager::CompletedCount() const
{
    int count(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   if(c.value()->GetStatus() == CALCS_COMPLETED) { count++; }
    }
    return count;
}

int CalculationManager::CanceledCount() const
{
    int count(0);
    for(CalculationHash::const_iterator c = _calculations.constBegin();
        c != _calculations.constEnd(); ++c)
    {   if(c.value()->GetStatus() == CALCS_CANCELED) { count++; }
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
