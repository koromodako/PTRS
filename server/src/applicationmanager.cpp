#include "applicationmanager.h"

#include "utils/logger.h"
#include "console/consolehandler.h"
#include "utils/calculationfactory.h"
#include "plugins/pluginmanager.h"

ApplicationManager ApplicationManager::_instance;

void ApplicationManager::Init()
{
    LOG_INFO("Initialisation des connexions SIG/SLOTS...");
    // -- initialisation des connexions pour la communication inter-threads
    // --- console_handler --> application_manager
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_EXEC(QByteArray)),        SLOT(SLOT_EXEC(QByteArray)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_STATUS()),                SLOT(SLOT_STATUS()));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_STATE()),                 SLOT(SLOT_STATE()));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_RESULT(QUuid,QString)),   SLOT(SLOT_RESULT(QUuid,QString)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_CANCEL(QUuid)),           SLOT(SLOT_CANCEL(QUuid)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_SHUTDOWN()),              SLOT(SLOT_SHUTDOWN()));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_TERMINATED()),            SLOT(SLOT_TERMINATED()));
    // --- application_mgr --> console_handler
    QObject::connect(this, SIGNAL(SIG_RESPONSE(Command,bool,QString)),
                     &(ConsoleHandler::GetInstance()), SLOT(SLOT_RESPONSE(Command,bool,QString)));

    LOG_INFO("Initialisation des composants...");
    // -- initialisation des composants
    if(!PluginManager::CheckPlugins())
    {   LOG_CRITICAL("Plugins integrity check failed !");
    }

    LOG_INFO("Démarrage du console handler...");
    // -- demarrage du thread d'interaction avec la console
    ConsoleHandler::GetInstance().start();
}

void ApplicationManager::SLOT_STATE()
{
    QString report = "\n"
                     "----------------- SERVER STATE REPORT -----------------\n"
                     "\n"
                     "Available plugins :\n";
    QStringList plugins = PluginManager::GetPluginsList();
    if(plugins.size() > 0)
    {   foreach (QString plugin, plugins)
        {   report += QString("  + %1\n").arg(plugin);
        }
    }
    else
    {   report += "  - no plugin available, sever is useless.\n";
    }
    report += "\n"
              "Calculation stats :\n"
              "  + scheduled : %1\n"
              "  + canceled  : %2\n"
              "  + completed : %3\n"
              "  + total     : %4\n"
              "\n"
              "Clients stats :\n"
              "  + available : %5\n"
              "  + working   : %6\n"
              "  + total     : %7\n"
              "\n"
              "Timing stats :\n"
              "  + calculation average lifetime : %8\n"
              "  + calculation average fragment count : %9\n"
              "\n"
              "-------------------------------------------------------";
    report = report.arg(QString::number(CalculationManager::getInstance().ScheduledCount()),
                        QString::number(CalculationManager::getInstance().CompletedCount()),
                        QString::number(CalculationManager::getInstance().CanceledCount()),
                        QString::number(CalculationManager::getInstance().Count()),
                        QString::number(-1), /// \todo complete here with NetworkManager stats
                        QString::number(-1), /// \todo complete here with NetworkManager stats
                        QString::number(-1), /// \todo complete here with NetworkManager stats
                        QString::number(CalculationManager::getInstance().AverageLifetime()),
                        QString::number(CalculationManager::getInstance().AverageFragmentCount()));
    emit SIG_RESPONSE(CMD_STATE, true, report);
}

void ApplicationManager::SLOT_EXEC(QByteArray json)
{
    QString error;
    Calculation * calculation = CalculationFactory::MakeCalculation(&_instance, json, error);
    if(calculation == NULL)
    {   emit SIG_RESPONSE(CMD_EXEC, false, error);
    }
    else
    {   if(CalculationManager::getInstance().Execute(calculation))
        {   emit SIG_RESPONSE(CMD_EXEC, true, QString("Calculation accepted id=%1. %2 fragments scheduled.").arg(
                                  calculation->GetId().toString(),
                                  calculation->GetFragmentCount()));
        }
        else
        {   emit SIG_RESPONSE(CMD_EXEC, false, "Missing binary for this calculation.");
        }
    }
}

void ApplicationManager::SLOT_STATUS()
{
    emit SIG_RESPONSE(CMD_STATUS, true, CalculationManager::getInstance().Status());
}

void ApplicationManager::SLOT_RESULT(QUuid id, QString filename)
{
    emit SIG_RESPONSE(CMD_RESULT, true, CalculationManager::getInstance().Result(id, filename));
}

void ApplicationManager::SLOT_CANCEL(QUuid id)
{
    if(CalculationManager::getInstance().Cancel(id))
    {   emit SIG_RESPONSE(CMD_CANCEL, true, QString("Calculation id=%1 scheduled for cancelation.").arg(id.toString()));
    }
    else
    {   emit SIG_RESPONSE(CMD_CANCEL, false, QString("Unknown calculation id=%1").arg(id.toString()));
    }
}

void ApplicationManager::SLOT_SHUTDOWN()
{
    LOG_DEBUG("SLOT_SHUTDOWN called");
    /// \todo implement here
    if(TERMINATED_EXPECTED_TOTAL - _terminated_ctr == 1)
    {   emit SIG_RESPONSE(CMD_SHUTDOWN, true, "STATUS command received !");
    }
}

void ApplicationManager::SLOT_TERMINATED()
{   _terminated_ctr++;
    if(_terminated_ctr >= TERMINATED_EXPECTED_TOTAL)
    {   emit SIG_TERMINATE();
    }
    else if(TERMINATED_EXPECTED_TOTAL - _terminated_ctr == 1)
    {   emit SIG_RESPONSE(CMD_SHUTDOWN, true, "STATUS command received !");
    }
    // emission du signal de terminaison quand tous les composants attendus ont notofié l'app manager de leur terminaison

}

ApplicationManager::ApplicationManager() :
    _terminated_ctr(0)
{

}
