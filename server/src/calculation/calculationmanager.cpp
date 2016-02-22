#include "calculationmanager.h"

CalculationManager CalculationManager::_instance;

bool CalculationManager::Execute(Calculation *calculation)
{
    /// \todo implement CalculationManager::Execute(Calculation *calculation)
    return false;
}

bool CalculationManager::Cancel(QUuid id)
{
    /// \todo implement CalculationManager::Cancel(QUuid id)
    return false;
}

QString CalculationManager::Result(QUuid id, QString filename) const
{
    /// \todo implement CalculationManager::Result(QUuid id, QString filename)
    return QString("We are working on this functionality.");
}

QString CalculationManager::Status() const
{
    /// \todo implement CalculationManager::Status()
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
    return -1;
}

int CalculationManager::AverageFragmentCount() const
{
    /// \todo implement CalculationManager::AverageFragmentCount()
    return -1;
}

CalculationManager::CalculationManager()
{}
