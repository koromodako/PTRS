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
        connect (calculation, SIGNAL(sig_stateUpdated(QUuid, Calculation::Status)),
                 this, SIGNAL(sig_calculationStateUpdated(QUuid, Calculation::Status)));
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

QString CalculationManager::Result(QUuid id, const QString & filename) const
{
    QString out("Error : no calculation match the given id.");
    CalculationHash::const_iterator it = _calculations.find(id);
    if(it != _calculations.end())
    {   QByteArray data = QJsonDocument(it.value()->GetResult()).toJson();
        if(!filename.isEmpty())
        {   QFile f(filename);
            if(f.open(QIODevice::WriteOnly))
            {
                f.write(data);  // write data to file
                f.close();      // close file
                out = "Info : data successfully written to output file.";
            }
            else
            {   LOG_ERROR("Error writing result file.");
                out = "Error : can't open output file to write data. Data : \n" + data;
            }
        }
        else
        {
            out = data;
        }
    }
    return out.prepend('\n');
}

QString CalculationManager::Status() const
{
    QString result("Calculations statuses :\n");
    for(CalculationHash::const_iterator c = _calculations.begin(); c != _calculations.end(); ++c)
    {
        result += QString("calculation(id=%1,state=%2)\n").arg(c.value()->GetId().toString(), Calculation::StatusToString(c.value()->GetStatus()));
    }
    if(_calculations.size() == 0)
    {   result = "No calculation result to display.";
    }
    return result.prepend('\n');
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

int CalculationManager::countWithState(Calculation::Status status) const
{
    int count(0);
    CalculationHash::const_iterator c;
    for(c = _calculations.constBegin() ; c != _calculations.constEnd() ; ++c)
    {
        if(c.value()->GetStatus() == status)
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

