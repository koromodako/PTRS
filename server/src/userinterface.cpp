#include "userinterface.h"

UserInterface::UserInterface(QObject *parent) : QObject(parent)
{

}

//default behaviors: ignore
void UserInterface::Slot_newClient(QUuid clientId) {}
void UserInterface::Slot_newCalculation(QUuid calculationId, QJsonDocument params) {}
void UserInterface::Slot_statusUpdated(QUuid id, Calculation::Status state) {}
void UserInterface::Slot_clientDisconnected(QUuid clientId) {}
