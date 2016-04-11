#include "widgetcalculs.h"
#include "../calculation/calculationmanager.h"
#include "addcalculationwindow.h"
#include "../utils/logger.h"


WidgetCalculs::WidgetCalculs(QWidget *parent) : QWidget(parent), addCalcWindow(NULL)
{

    // --- Layout
    QVBoxLayout *layout = new QVBoxLayout();

    // --- Initialisation du tableau
    tableWidget = new QTableWidget(0, 6, this);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //tableWidget->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Taille des colonnes
    tableWidget->setColumnWidth(0, 90);
    for (int c = 1; c < tableWidget->horizontalHeader()->count(); ++c)
    {
        tableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    // Headers du tableau
    QTableWidgetItem * titreCalculationId = new QTableWidgetItem("Calculation ID");
    QTableWidgetItem * titreCalculationName = new QTableWidgetItem("Calculation Name");
    QTableWidgetItem * titreCalculationStatus = new QTableWidgetItem("Calculation Status");
    QTableWidgetItem * titreFragmentCount = new QTableWidgetItem("Fragment Count");
    QTableWidgetItem * titreClientsWorking = new QTableWidgetItem("Clients Working");
    QTableWidgetItem * titreActionButtons = new QTableWidgetItem("");
    tableWidget->setHorizontalHeaderItem(0, titreCalculationId);
    tableWidget->setHorizontalHeaderItem(1, titreCalculationName);
    tableWidget->setHorizontalHeaderItem(2, titreCalculationStatus);
    tableWidget->setHorizontalHeaderItem(3, titreFragmentCount);
    tableWidget->setHorizontalHeaderItem(4, titreClientsWorking);
    tableWidget->setHorizontalHeaderItem(5, titreActionButtons);
    layout->addWidget(tableWidget);

    // Bouton Ajout Calcul
    QPushButton * newCalc = new QPushButton("New Calculation");
    newCalc->setMaximumWidth(160);
    layout->addWidget(newCalc);
    layout->setAlignment(newCalc, Qt::AlignRight);

    // Signaux
    connect(newCalc, SIGNAL(clicked()), this, SLOT(Slot_AddNewCalculation()));
    connect(&CalculationManager::getInstance(), SIGNAL(sig_newCalculation(QUuid)), this, SLOT(Slot_NewCalculation(QUuid)));
    connect(&CalculationManager::getInstance(), SIGNAL(sig_calculationStateUpdated(QUuid, Calculation::State)),
            this, SLOT(Slot_StateUpdated(QUuid, Calculation::State)));

    // Layout
    this->setLayout(layout);

}

void WidgetCalculs::Slot_AddNewCalculation()
{
    if(addCalcWindow != NULL)
    {
        addCalcWindow->showAndReset();
    }
    else
    {
        addCalcWindow = new AddCalculationWindow(this);
        addCalcWindow->show();
    }
}

void WidgetCalculs::Slot_NewCalculation(QUuid id)
{
    LOG_DEBUG("Slot_NewCalculation called (QUid : " + id.toString() + ")");

    memorisationPositions.insert(id, tableWidget->rowCount());
    tableWidget->insertRow(tableWidget->rowCount());
    QTableWidgetItem * idTab = new QTableWidgetItem(id.toString().remove(0,1).remove(id.toString().length() - 2, 1));
    tableWidget->setItem(tableWidget->rowCount() - 1, 0, idTab);
}

void WidgetCalculs::Slot_StateUpdated(QUuid id, Calculation::State state)
{
    const QMetaObject &mo = Calculation::staticMetaObject;
    int index = mo.indexOfEnumerator("State");
    QMetaEnum stateEnum = mo.enumerator(index);
    QString stateString = QString(stateEnum.valueToKey(state));

    LOG_DEBUG("Updating calculation state (id : " + id.toString() + ", row : "
              + QString::number(memorisationPositions.value(id)) + ", state : " + stateString);
    QTableWidgetItem * stateTab = new QTableWidgetItem(stateString);
    tableWidget->setItem(memorisationPositions.value(id), 2, stateTab);
}
