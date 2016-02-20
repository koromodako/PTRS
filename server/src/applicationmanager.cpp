#include "applicationmanager.h"

#include "utils/logger.h"
#include "console/consolehandler.h"

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

void ApplicationManager::SLOT_EXEC(QByteArray calculationOrderJSON)
{
    LOG_DEBUG("SLOT_EXEC called");
    /// \todo implement here
    emit SIG_RESPONSE(CMD_EXEC, true, "EXEC command received !");
}

void ApplicationManager::SLOT_STATUS()
{
    LOG_DEBUG("SLOT_STATUS called");
    /// \todo implement here
    emit SIG_RESPONSE(CMD_STATUS, true, "STATUS command received !");
}

void ApplicationManager::SLOT_RESULT(QUuid id, QString filename)
{
    LOG_DEBUG("SLOT_RESULT called");
    /// \todo implement here
    emit SIG_RESPONSE(CMD_RESULT, true, "RESULT command received !");
}

void ApplicationManager::SLOT_CANCEL(QUuid id)
{
    LOG_DEBUG("SLOT_CANCEL called");
    /// \todo implement here
    emit SIG_RESPONSE(CMD_CANCEL, true, "CANCEL command received !");
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
