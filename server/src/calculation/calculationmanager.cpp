#include "calculationmanager.h"

CalculationManager CalculationManager::_instance;

bool CalculationManager::Execute(Calculation *calculation)
{
    /// \todo implement CalculationManager::Execute(Calculation *calculation)
}

bool CalculationManager::Cancel(QUuid id)
{
    /// \todo implement CalculationManager::Cancel(QUuid id)
}

QString CalculationManager::Result(QUuid id, QString filename) const
{
    /// \todo implement CalculationManager::Result(QUuid id, QString filename)
}

QString CalculationManager::Status() const
{
    /// \todo implement CalculationManager::Status()
}

CalculationManager::CalculationManager()
{}
