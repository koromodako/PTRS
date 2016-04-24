#include "mainwindowcontroller.h"
#include "../utils/logger.h"
#include "../calculation/specs.h"
#include "../calculation/calculation.h"

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

void MainWindowController::Slot_newClient(QUuid clientId)
{
    window.getGraphWidget()->newClient(clientId);
}

void MainWindowController::Slot_newCalculation(QUuid calculationId, QJsonDocument params)
{
    window.getGraphWidget()->newCalculation(calculationId, params.object()[CS_JSON_KEY_CALC_BIN].toString());
}

void MainWindowController::Slot_stateUpdated(QUuid id, Calculation::Status state)
{
    if(state == Calculation::COMPLETED || state == Calculation::CANCELED
            || state == Calculation::CRASHED)
    {
        window.getGraphWidget()->deleteCalculation(id);
    }
}

void MainWindowController::Slot_clientWorkingOnCalculation(QUuid calculationId, QUuid clientId)
{
    window.getGraphWidget()->clientWorkingOnCalculation(calculationId, clientId);
}
