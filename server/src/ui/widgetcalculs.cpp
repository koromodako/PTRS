#include "widgetcalculs.h"
#include "../calculation/calculationmanager.h"
#include "addcalculationwindow.h"
#include "../utils/logger.h"


WidgetCalculs::WidgetCalculs(QWidget *parent) : QWidget(parent), addCalcWindow(NULL)
{

    // --- Layout
    QVBoxLayout *layout = new QVBoxLayout();

    // --- Initialisation du tableau
    tableWidget = new QTableWidget(0, 7, this);
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
    QTableWidgetItem * titreFragmentCount = new QTableWidgetItem("Overall Progress");
    QTableWidgetItem * titreClientsWorking = new QTableWidgetItem("Clients Working");
    QTableWidgetItem * titreAnnuler = new QTableWidgetItem("");
    QTableWidgetItem * titreResultat = new QTableWidgetItem("");
    tableWidget->setHorizontalHeaderItem(C_ID, titreCalculationId);
    tableWidget->setHorizontalHeaderItem(C_NOM, titreCalculationName);
    tableWidget->setHorizontalHeaderItem(C_STATUT, titreCalculationStatus);
    tableWidget->setHorizontalHeaderItem(C_PROGRES, titreFragmentCount);
    tableWidget->setHorizontalHeaderItem(C_CLIENTS, titreClientsWorking);
    tableWidget->setHorizontalHeaderItem(C_ANNULER, titreAnnuler);
    tableWidget->setHorizontalHeaderItem(C_RESULTAT, titreResultat);
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
    connect(&CalculationManager::getInstance(), SIGNAL(sig_calculationProgressUpdated(QUuid,int)), this,
                                                       SLOT(Slot_ProgressUpdated(QUuid,int)));

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
    LOG_DEBUG("New calculation added (QUid : " + id.toString() + ")");

    memorisationPositions.insert(id, tableWidget->rowCount());
    tableWidget->insertRow(tableWidget->rowCount());
    QTableWidgetItem * idTab = new QTableWidgetItem(id.toString().remove(0,1).remove(id.toString().length() - 2, 1));
    idTab->setTextAlignment(Qt::AlignCenter);
    tableWidget->setItem(tableWidget->rowCount() - 1, C_ID, idTab);

    tableWidget->setCellWidget(tableWidget->rowCount() - 1, C_PROGRES, new QProgressBar());

    QPushButton * annuler = new QPushButton("Cancel");
    tableWidget->setCellWidget(tableWidget->rowCount() - 1, C_ANNULER, annuler);
}

void WidgetCalculs::Slot_StateUpdated(QUuid id, Calculation::State state)
{
    const QMetaObject &mo = Calculation::staticMetaObject;
    int index = mo.indexOfEnumerator("State");
    QMetaEnum stateEnum = mo.enumerator(index);
    QString stateString = QString(stateEnum.valueToKey(state));

    LOG_DEBUG("Updating calculation state (id : " + id.toString() + ", row : "
              + QString::number(memorisationPositions.value(id)) + ", state : " + stateString);

    QTableWidgetItem * stateTab = new QTableWidgetItem();
    stateTab->setText(stateString);
    stateTab->setTextAlignment(Qt::AlignCenter);
    tableWidget->setItem(memorisationPositions.value(id), C_STATUT, stateTab);
}

void WidgetCalculs::Slot_ProgressUpdated(QUuid id, int value)
{
    LOG_DEBUG("Updating progress (QUid : " + id.toString() + ", progress : " + value  + ")");

    QProgressBar * progres = new QProgressBar();
    progres->setValue(value);
    tableWidget->setCellWidget(memorisationPositions.value(id), C_PROGRES, progres);
}


