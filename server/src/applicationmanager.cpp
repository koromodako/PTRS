#include "applicationmanager.h"

#include "utils/logger.h"
#include "console/consolehandler.h"
#include "utils/calculationfactory.h"

ApplicationManager ApplicationManager::_instance;

void ApplicationManager::Init()
{
    LOG_INFO("Initialisation des connexions SIG/SLOTS...");
    // -- initialisation des connexions pour la communication inter-threads
    // --- console_handler --> application_manager
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_EXEC(QByteArray)),        SLOT(SLOT_EXEC(QByteArray)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_STATUS()),                SLOT(SLOT_STATUS()));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_RESULT(QUuid,QString)),   SLOT(SLOT_RESULT(QUuid,QString)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_CANCEL(QUuid)),           SLOT(SLOT_CANCEL(QUuid)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_SHUTDOWN()),              SLOT(SLOT_SHUTDOWN()));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_TERMINATED()),            SLOT(SLOT_TERMINATED()));
    // --- application_mgr --> console_handler
    QObject::connect(this, SIGNAL(SIG_RESPONSE(Command,bool,QString)),
                     &(ConsoleHandler::GetInstance()), SLOT(SLOT_RESPONSE(Command,bool,QString)));

    LOG_INFO("Initialisation des composants...");
    // -- initialisation des composants
    // rien à faire ici pour l'instant

    LOG_INFO("Démarrage du console handler...");
    // -- demarrage du thread d'interaction avec la console
    ConsoleHandler::GetInstance().start();
}

void ApplicationManager::SLOT_STATE()
{
    LOG_DEBUG("SLOT_STATE called");
    /// \todo implement here
    emit SIG_RESPONSE(CMD_STATE, true, "STATE command received !");
}

void ApplicationManager::SLOT_EXEC(QByteArray json)
{
    Calculation * calculation = CalculationFactory::MakeCalculation(&_instance, json);
    if(CalculationManager::getInstance().Execute(calculation))
    {   emit SIG_RESPONSE(CMD_EXEC, true, QString("Calculation accepted id=%1. %2 fragments scheduled.").arg(
                              calculation->GetId().toString(),
                              calculation->GetFragmentCount()));
    }
    else
    {   emit SIG_RESPONSE(CMD_EXEC, false, "Missing binary for this calculation.");
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
    emit SIG_RESPONSE(CMD_SHUTDOWN, true, "STATUS command received !");
}

void ApplicationManager::SLOT_TERMINATED()
{   _terminated_ctr++;
    if(_terminated_ctr >= TERMINATED_EXPECTED_TOTAL)
    // emission du signal de terminaison quand tous les composants attendus ont notofié l'app manager de leur terminaison
    emit SIG_TERMINATE();
}

ApplicationManager::ApplicationManager() :
    _terminated_ctr(0)
{

}
