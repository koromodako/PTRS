#include <QApplication>

#include "src/utils/logger.h"
#include "console/consolehandler.h"
#include "applicationmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LOGGER_CONFIGURE(LVL_NO_LVL, LOG_FORMAT_DETAILED);

    ApplicationManager::GetInstance().Init();

    QObject::connect(&(ApplicationManager::GetInstance()), SIGNAL(SIG_TERMINATE()),
                     qApp, SLOT(quit()));

    return a.exec();
}