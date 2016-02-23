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
    if(PluginManager::getInstance().PluginExists(calculation->GetBin()))
    {   // - construction des paramètres
        // -- recuperation du nom du binaire
        QString bin = calculation->GetBin();
        // -- construction des arguments a passer au binaire
        QStringList args;
        args << CS_OP_SPLIT << calculation->ToJson();
        // -- construction des objet string pour récupération des sorties du plugin
        QString out, err;
        if(PluginManager::getInstance().RunPlugin(bin, args, out, err))
        {   LOG_INFO("Plugin exited normally.");
            // treat plugin output
            QString parsing_err;
            foreach (QString json_fragment, out.split(CS_FRAGMENT_SEP))
            {   Calculation * fragment = Calculation::FromJson(calculation, json_fragment.toUtf8(), parsing_err);
                if(fragment == NULL)
                {   LOG_ERROR("Calculation factory failed to parse fragment.");
                    LOG_ERROR(parsing_err);
                    ok = false;
                    break; // interrupt here
                }
                else
                {   calculation->AddFragment(fragment);
                }
                ok = true;
            }
        }
        else
        {   LOG_ERROR("Plugin exited abnormally.");
            LOG_ERROR(calculation->dump());
            LOG_ERROR(QString("Output is : ").append(out));
            LOG_ERROR(QString("Errput is : ").append(err));
        }
    }
    else
    {   LOG_ERROR("Call Execute on a missing plugin !");
        LOG_ERROR(calculation->dump());
    }
    return ok;
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
