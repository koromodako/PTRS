#include "applicationmanager.h"

#include "utils/logger.h"
#include "console/consolehandler.h"
#include "plugins/pluginmanager.h"
#include "ui/mainwindow.h"

ApplicationManager ApplicationManager::_instance;

void ApplicationManager::Init(UserInterface *interface)
{
    ConsoleHandler::getInstance().moveToThread(&_consoleThread);
    NetworkManager::getInstance().moveToThread(&_networkThread);

    LOG_INFO("Initialisation des connexions signaux/slots...");

    // -- initialisation des connexions pour la communication inter-threads
    connect(&_consoleThread, &QThread::started, interface, &UserInterface::Slot_init);
    connect(&_networkThread, &QThread::started, &NetworkManager::getInstance(), &NetworkManager::Slot_init);
    connect(&NetworkManager::getInstance(), SIGNAL(sig_started()), &_consoleThread, SLOT(start()));

    // --- console_handler --> application_manager
    connect(interface, SIGNAL(sig_exec(QByteArray)),        SLOT(Slot_exec(QByteArray)));
    connect(interface, SIGNAL(sig_status()),                SLOT(Slot_status()));
    connect(interface, SIGNAL(sig_state()),                 SLOT(Slot_state()));
    connect(interface, SIGNAL(sig_result(QUuid,QString)),   SLOT(Slot_result(QUuid,QString)));
    connect(interface, SIGNAL(sig_cancel(QUuid)),           SLOT(Slot_cancel(QUuid)));
    connect(interface, SIGNAL(sig_shutdown()),              SLOT(Slot_shutdown()));
    connect(interface, SIGNAL(sig_terminated()),            SLOT(Slot_terminated()));
    // --- application_mgr --> console_handler
    connect(this, SIGNAL(sig_response(Command,bool,QString)),
            interface, SLOT(Slot_response(Command,bool,QString)));
    // --- plugin_mgr --> application_mgr
    connect(&(PluginManager::getInstance()), SIGNAL(sig_terminated()), SLOT(Slot_terminated()));
    // --- application_mgr --> plugin_mgr
    connect(this, SIGNAL(sig_terminateModule()),
            &(PluginManager::getInstance()), SLOT(Slot_terminate()));

    //network_manager --> userinterface
    connect(&(NetworkManager::getInstance()), SIGNAL(sig_newClient(QUuid)), interface, SLOT(Slot_newClient(QUuid)));

    //calc_manager --> userinterface
    connect(&(CalculationManager::getInstance()), SIGNAL(sig_newCalculation(QUuid,QJsonDocument)),
                interface, SLOT(Slot_newCalculation(QUuid,QJsonDocument)));
    connect(&(CalculationManager::getInstance()), SIGNAL(sig_calculationStateUpdated(QUuid,Calculation::Status)),
                interface, SLOT(Slot_stateUpdated(QUuid,Calculation::Status)));

    LOG_INFO("Initialisation des composants...");
    // -- initialisation des composants
    PluginManager::getInstance().Init();
    if(!PluginManager::getInstance().CheckPlugins())
    {   LOG_CRITICAL("Plugins integrity check failed !");
    }

    _networkThread.start();
}

void ApplicationManager::Slot_state()
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
            .arg(NetworkManager::getInstance().AvailableClientCount())
            .arg(NetworkManager::getInstance().WorkingClientCount())
            .arg(NetworkManager::getInstance().ClientCount())
            .arg(CalculationManager::getInstance().AverageLifetime())
            .arg(CalculationManager::getInstance().AverageFragmentCount());
    LOG_DEBUG("sig_response(CMD_STATE) emitted.");
    emit sig_response(CMD_STATE, true, report);
}

void ApplicationManager::Slot_exec(QByteArray json)
{
    QString error;
    Calculation * calculation = Calculation::FromJson(&_instance, json, error);
    if(calculation == NULL)
    {   LOG_DEBUG("sig_response(CMD_EXEC,false) emitted.");
        emit sig_response(CMD_EXEC, false, error);
    }
    else
    {   if(CalculationManager::getInstance().Execute(calculation, json))
        {   LOG_DEBUG("sig_response(CMD_EXEC,true) emitted.");
            emit sig_response(CMD_EXEC, true, QString("Calculation accepted id=%1.").arg(
                                  calculation->GetId().toString()));
        }
        else
        {   LOG_DEBUG("sig_response(CMD_EXEC,false) emitted.");
            emit sig_response(CMD_EXEC, false, "Missing binary for this calculation.");
        }
    }
}

void ApplicationManager::Slot_status()
{   LOG_DEBUG("Slot_status() called.");
    LOG_DEBUG("sig_response(CMD_STATUS) emitted.");
    emit sig_response(CMD_STATUS, true, CalculationManager::getInstance().Status());
}

void ApplicationManager::Slot_result(QUuid id, QString filename)
{   LOG_DEBUG("Slot_result() called.");
    LOG_DEBUG("sig_response(CMD_RESULT) emitted.");
    emit sig_response(CMD_RESULT, true, CalculationManager::getInstance().Result(id, filename));
}

void ApplicationManager::Slot_cancel(QUuid id)
{   LOG_DEBUG("Slot_cancel() called.");
    if(CalculationManager::getInstance().Cancel(id))
    {   LOG_DEBUG("sig_response(CMD_CANCEL,true) emitted.");
        emit sig_response(CMD_CANCEL, true, QString("Calculation id=%1 scheduled for cancelation.").arg(id.toString()));
    }
    else
    {   LOG_DEBUG("sig_response(CMD_CANCEL,false) emitted.");
        emit sig_response(CMD_CANCEL, false, QString("Unknown calculation id=%1").arg(id.toString()));
    }
}

void ApplicationManager::Slot_shutdown()
{   LOG_DEBUG("Slot_shutdown() called.");
    emit sig_terminateModule();
#if TERMINATED_EXPECTED_TOTAL == 1
    if(TERMINATED_EXPECTED_TOTAL - _terminated_ctr == 1)
    {   LOG_DEBUG("sig_response(CMD_SHUTDOWN) emitted.");
        emit SIG_RESPONSE(CMD_SHUTDOWN, true, "SHUTDOWN command received !");
    }
#endif
}

void ApplicationManager::Slot_terminated()
{   LOG_DEBUG("Slot_terminated() called.");
    _terminatedCtr++;
    if(_terminatedCtr >= TERMINATED_EXPECTED_TOTAL)
    {   LOG_DEBUG("sig_terminated() emitted.");

        _networkThread.quit();
        _networkThread.wait();
        _consoleThread.quit();
        _consoleThread.wait();
        // emission du signal de terminaison quand tous les composants attendus ont notofié l'app manager de leur terminaison
        emit sig_terminated();
    }
    else if(TERMINATED_EXPECTED_TOTAL - _terminatedCtr == 1)
    {   LOG_DEBUG("sig_response(CMD_SHUTDOWN) emitted.");
        emit sig_response(CMD_SHUTDOWN, true, "SHUTDOWN command received !");
    }
}

ApplicationManager::ApplicationManager() :
    _terminatedCtr(0)
{
}

ApplicationManager::~ApplicationManager()
{

}
