#include <QApplication>

#include "src/utils/logger.h"
#include "console/consolehandler.h"
#include "applicationmanager.h"
#include "src/network/networkmanager.h"
#include "src/const.h"
#include "src/userinterface.h"

#include "ui/mainwindowcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserInterface *interface;

    if(argc == 2)
    {
        MainWindowController::CreateInstance();
        interface = MainWindowController::GetInstance();
    }
    else
    {
        interface = &ConsoleHandler::getInstance();
    }

    LOGGER_CONFIGURE(LVL_NO_LVL, LOG_FORMAT_DETAILED);

    qRegisterMetaType<Command>("Command");

    ApplicationManager::GetInstance().Init(interface);

    QObject::connect(&(ApplicationManager::GetInstance()), SIGNAL(sig_terminated()),
                     qApp, SLOT(quit()));

    return a.exec();
}
