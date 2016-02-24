#include "applicationmanager.h"

#include "utils/logger.h"
#include "console/consolehandler.h"
#include "plugins/pluginmanager.h"

ApplicationManager ApplicationManager::_instance;

void ApplicationManager::Init()
{
    LOG_INFO("Initialisation des connexions SIG/SLOTS...");
    // -- initialisation des connexions pour la communication inter-threads
    // --- console_handler --> application_manager
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_EXEC(QByteArray)),        SLOT(SLOT_EXEC(QByteArray)));
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_STATUS()),                SLOT(SLOT_STATUS()));
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_STATE()),                 SLOT(SLOT_STATE()));
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_RESULT(QUuid,QString)),   SLOT(SLOT_RESULT(QUuid,QString)));
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_CANCEL(QUuid)),           SLOT(SLOT_CANCEL(QUuid)));
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_SHUTDOWN()),              SLOT(SLOT_SHUTDOWN()));
    connect(&(ConsoleHandler::getInstance()), SIGNAL(SIG_TERMINATED()),            SLOT(SLOT_TERMINATED()));
    // --- application_mgr --> console_handler
    connect(this, SIGNAL(SIG_RESPONSE(Command,bool,QString)),
            &(ConsoleHandler::getInstance()), SLOT(SLOT_RESPONSE(Command,bool,QString)));
    // --- plugin_mgr --> application_mgr
    connect(&(PluginManager::getInstance()), SIGNAL(SIG_TERMINATED()), SLOT(SLOT_TERMINATED()));
    // --- application_mgr --> plugin_mgr
    connect(this, SIGNAL(SIG_TERMINATE_MODULE()),
            &(PluginManager::getInstance()), SLOT(SLOT_TERMINATE()));

    LOG_INFO("Initialisation des composants...");
    // -- initialisation des composants
    PluginManager::getInstance().Init();
    if(!PluginManager::getInstance().CheckPlugins())
    {   LOG_CRITICAL("Plugins integrity check failed !");
    }

    LOG_INFO("Démarrage du console handler...");
    // -- demarrage du thread d'interaction avec la console
    ConsoleHandler::getInstance().start();
}

void ApplicationManager::SLOT_STATE()
{
    QString report = "\n"
                     "----------------- SERVER STATE REPORT -----------------\n"
                     "\n"
                     "Available plugins :\n";
    QStringList plugins = PluginManager::getInstance().GetPluginsList();
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
              "  + crashed   : %3\n"
              "  + completed : %4\n"
              "  + total     : %5\n"
              "\n"
              "Clients stats :\n"
              "  + available : %6\n"
              "  + working   : %7\n"
              "  + total     : %8\n"
              "\n"
              "Timing stats :\n"
              "  + calculation average lifetime : %9\n"
              "  + calculation average fragment count : %10\n"
              "\n"
              "-------------------------------------------------------";
    report = report
            .arg(CalculationManager::getInstance().ScheduledCount())
            .arg(CalculationManager::getInstance().CanceledCount())
            .arg(CalculationManager::getInstance().CrashedCount())
            .arg(CalculationManager::getInstance().CompletedCount())
            .arg(CalculationManager::getInstance().Count())
            .arg(-1) /// \todo complete here with NetworkManager stats
            .arg(-1) /// \todo complete here with NetworkManager stats
            .arg(-1) /// \todo complete here with NetworkManager stats
            .arg(CalculationManager::getInstance().AverageLifetime())
            .arg(CalculationManager::getInstance().AverageFragmentCount());
    LOG_DEBUG("SIG_RESPONSE(CMD_STATE) emitted.");
    emit SIG_RESPONSE(CMD_STATE, true, report);
}

void ApplicationManager::SLOT_EXEC(QByteArray json)
{
    QString error;
    Calculation * calculation = Calculation::FromJson(&_instance, json, error);
    if(calculation == NULL)
    {   LOG_DEBUG("SIG_RESPONSE(CMD_EXEC,false) emitted.");
        emit SIG_RESPONSE(CMD_EXEC, false, error);
    }
    else
    {   if(CalculationManager::getInstance().Execute(calculation))
        {   LOG_DEBUG("SIG_RESPONSE(CMD_EXEC,true) emitted.");
            emit SIG_RESPONSE(CMD_EXEC, true, QString("Calculation accepted id=%1.").arg(
                                  calculation->GetId().toString()));
        }
        else
        {   LOG_DEBUG("SIG_RESPONSE(CMD_EXEC,false) emitted.");
            emit SIG_RESPONSE(CMD_EXEC, false, "Missing binary for this calculation.");
        }
    }
}

void ApplicationManager::SLOT_STATUS()
{   LOG_DEBUG("SLOT_STATUS() called.");
    LOG_DEBUG("SIG_RESPONSE(CMD_STATUS) emitted.");
    emit SIG_RESPONSE(CMD_STATUS, true, CalculationManager::getInstance().Status());
}

void ApplicationManager::SLOT_RESULT(QUuid id, QString filename)
{   LOG_DEBUG("SLOT_RESULT() called.");
    LOG_DEBUG("SIG_RESPONSE(CMD_RESULT) emitted.");
    emit SIG_RESPONSE(CMD_RESULT, true, CalculationManager::getInstance().Result(id, filename));
}

void ApplicationManager::SLOT_CANCEL(QUuid id)
{   LOG_DEBUG("SLOT_CANCEL() called.");
    if(CalculationManager::getInstance().Cancel(id))
    {   LOG_DEBUG("SIG_RESPONSE(CMD_CANCEL,true) emitted.");
        emit SIG_RESPONSE(CMD_CANCEL, true, QString("Calculation id=%1 scheduled for cancelation.").arg(id.toString()));
    }
    else
    {   LOG_DEBUG("SIG_RESPONSE(CMD_CANCEL,false) emitted.");
        emit SIG_RESPONSE(CMD_CANCEL, false, QString("Unknown calculation id=%1").arg(id.toString()));
    }
}

void ApplicationManager::SLOT_SHUTDOWN()
{   LOG_DEBUG("SLOT_SHUTDOWN() called.");
    emit SIG_TERMINATE_MODULE();
#if TERMINATED_EXPECTED_TOTAL == 1
    if(TERMINATED_EXPECTED_TOTAL - _terminated_ctr == 1)
    {   LOG_DEBUG("SIG_RESPONSE(CMD_SHUTDOWN) emitted.");
        emit SIG_RESPONSE(CMD_SHUTDOWN, true, "STATUS command received !");
    }
#endif
}

void ApplicationManager::SLOT_TERMINATED()
{   LOG_DEBUG("SLOT_TERMINATED() called.");
    _terminated_ctr++;
    if(_terminated_ctr >= TERMINATED_EXPECTED_TOTAL)
    {   LOG_DEBUG("SIG_TERMINATE() emitted.");
        emit SIG_TERMINATE();
    }
    else if(TERMINATED_EXPECTED_TOTAL - _terminated_ctr == 1)
    {   LOG_DEBUG("SIG_RESPONSE(CMD_SHUTDOWN) emitted.");
        emit SIG_RESPONSE(CMD_SHUTDOWN, true, "STATUS command received !");
    }
    // emission du signal de terminaison quand tous les composants attendus ont notofié l'app manager de leur terminaison

}

ApplicationManager::ApplicationManager() :
    _terminated_ctr(0)
{

}
