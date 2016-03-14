#include <QCoreApplication>

#include "src/network/clientsession.h"
#include "src/utils/logger.h"
#include "console/consolehandler.h"
#include "applicationmanager.h"
#include "src/const.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LOGGER_CONFIGURE(LVL_NO_LVL, LOG_FORMAT_DETAILED);

    qRegisterMetaType<Command>("Command");

    ApplicationManager::GetInstance().Init();

    QObject::connect(&(ApplicationManager::GetInstance()), SIGNAL(sig_terminated()),
                     qApp, SLOT(quit()));

    //ClientSession session;

    return a.exec();
}
