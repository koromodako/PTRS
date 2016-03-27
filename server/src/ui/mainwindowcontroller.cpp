#include "mainwindowcontroller.h"
#include "../utils/logger.h"

MainWindowController *MainWindowController::_instance = NULL;

MainWindowController::MainWindowController(QObject *parent)
    : UserInterface(parent), window()
{

}

void MainWindowController::windowClosed()
{
    emit sig_shutdown();
}

void MainWindowController::Slot_init()
{
    LOG_DEBUG("Slot_init() received by UI, showing window.");

    window.show();
}

void MainWindowController::Slot_response(Command command, bool ok, QString message)
{
    if(command == CMD_SHUTDOWN) {
        window.hide();
        // notify termination
        emit sig_terminated();
    } else {
        LOG_ERROR("Slot_response is not implemented.");
    }
}
