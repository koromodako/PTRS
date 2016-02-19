#include <QApplication>

#include "src/utils/logger.h"
#include "console/consolehandler.h"
#include "network/networkmanager.h"
#include "calculation/calculationmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LOG_INFO("Initialisation des connexions SIG/SLOTS...");
    // -- initialisation des connexions pour la communication inter-threads
    // --- console_handler --> calculation_manager
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_EXEC(CalculationOrder&)),
                     &(CalculationManager::GetInstance()), SLOT(SLOT_EXEC(CalculationOrder&)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_STATUS()),
                     &(CalculationManager::GetInstance()), SLOT(SLOT_STATUS()));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_RESULT(uint)),
                     &(CalculationManager::GetInstance()), SLOT(SLOT_RESULT(uint)));
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_CANCEL(uint)),
                     &(CalculationManager::GetInstance()), SLOT(SLOT_CANCEL(uint)));
    // --- calculation_mgr --> console_handler
    QObject::connect(&(CalculationManager::GetInstance()), SIGNAL(SIG_RESPONSE(Command,bool,QList<Calculation>&)),
                     &(ConsoleHandler::GetInstance()), SLOT(SLOT_RESPONSE(Command,bool,QList<Calculation>&)));
    // --- console_handler --> qApp
    QObject::connect(&(ConsoleHandler::GetInstance()), SIGNAL(SIG_SHUTDOWN()), qApp, SLOT(quit()));

    LOG_INFO("Initialisation des composants...");
    // -- initialisation des composants
    // rien à faire ici pour l'instant

    LOG_INFO("Démarrage du console handler...");
    // -- demarrage du thread d'interaction avec la console
    ConsoleHandler::GetInstance().start();

    return a.exec();
}
