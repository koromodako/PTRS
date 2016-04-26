#include "mainwindowcontroller.h"
#include "../utils/logger.h"
#include "../calculation/specs.h"
#include "../calculation/calculation.h"

#include <QMessageBox>

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
    if(command == CMD_SHUTDOWN)
    {
        window.hide();
        // notify termination
        emit sig_terminated();
    }
    else if(command == CMD_EXEC) {
        if(ok)
        {
            window.closeAddCalculationWindow();
        }
        else
        {
            QMessageBox *box = new QMessageBox(&window);
            box->setText("An error occured while running calculation.\n" + message);
            box->show();
        }
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

void MainWindowController::Slot_statusUpdated(QUuid id, Calculation::Status state)
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

void MainWindowController::Slot_clientDisconnected(QUuid clientId)
{
    window.getGraphWidget()->deleteClient(clientId);
}
