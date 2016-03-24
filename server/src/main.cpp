#include <QApplication>

#include "src/utils/logger.h"
#include "console/consolehandler.h"
#include "applicationmanager.h"
#include "src/network/networkmanager.h"
#include "src/const.h"

#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow::CreateInstance()->show();

    LOGGER_CONFIGURE(LVL_NO_LVL, LOG_FORMAT_DETAILED);

    qRegisterMetaType<Command>("Command");

    ApplicationManager::GetInstance().Init();

    QObject::connect(&(ApplicationManager::GetInstance()), SIGNAL(sig_terminated()),
                     qApp, SLOT(quit()));

    return a.exec();
}
