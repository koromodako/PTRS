#include "widgetcalculs.h"
#include "../calculation/specs.h"
#include "../calculation/calculationmanager.h"
#include "addcalculationwindow.h"
#include "../utils/logger.h"
#include "../ui/mainwindowcontroller.h"


WidgetCalculs::WidgetCalculs(QWidget *parent) : QWidget(parent), addCalcWindow(NULL)
{

    // --- Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

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
    QTableWidgetItem * titreTarget = new QTableWidgetItem("");
    QTableWidgetItem * titreAnnuler = new QTableWidgetItem("");
    QTableWidgetItem * titreResultat = new QTableWidgetItem("");
    tableWidget->setHorizontalHeaderItem(C_ID, titreCalculationId);
    tableWidget->setHorizontalHeaderItem(C_NOM, titreCalculationName);
    tableWidget->setHorizontalHeaderItem(C_STATUT, titreCalculationStatus);
    tableWidget->setHorizontalHeaderItem(C_PROGRES, titreFragmentCount);
    tableWidget->setHorizontalHeaderItem(C_PARAMS, titreTarget);
    tableWidget->setHorizontalHeaderItem(C_ANNULER, titreAnnuler);
    tableWidget->setHorizontalHeaderItem(C_RESULTAT, titreResultat);
    layout->addWidget(tableWidget);

    // Bouton Ajout Calcul
    QPushButton * newCalc = new QPushButton("New Calculation", this);
    newCalc->setMaximumWidth(160);
    layout->addWidget(newCalc);
    layout->setAlignment(newCalc, Qt::AlignRight);

    // Signaux
    connect(newCalc, SIGNAL(clicked()), this, SLOT(Slot_AddNewCalculation()));
    connect(&CalculationManager::getInstance(), SIGNAL(sig_newCalculation(QUuid, QJsonDocument)), this,
            SLOT(Slot_NewCalculation(QUuid, QJsonDocument)));
    connect(&CalculationManager::getInstance(), SIGNAL(sig_calculationStateUpdated(QUuid, Calculation::Status)),
            this, SLOT(Slot_StateUpdated(QUuid, Calculation::Status)));
    connect(&CalculationManager::getInstance(), SIGNAL(sig_calculationProgressUpdated(QUuid,int)), this,
                                                       SLOT(Slot_ProgressUpdated(QUuid,int)));
    connect(&CalculationManager::getInstance(), SIGNAL(sig_calculationDone(QUuid, QJsonObject)), this,
                                                       SLOT(Slot_CalculationDone(QUuid, QJsonObject)));
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

void WidgetCalculs::Slot_NewCalculation(QUuid id, QJsonDocument params)
{
    LOG_DEBUG("New calculation added (QUid : " + id.toString() + ")");

    memIdToRow.insert(id, tableWidget->rowCount());
    tableWidget->insertRow(tableWidget->rowCount());
    int ligneInsertion = tableWidget->rowCount() - 1;

    QTableWidgetItem * idTab = new QTableWidgetItem(id.toString().remove(0,1).remove(id.toString().length() - 2, 1));
    idTab->setTextAlignment(Qt::AlignCenter);
    tableWidget->setItem(ligneInsertion, C_ID, idTab);

    tableWidget->setCellWidget(ligneInsertion, C_PROGRES, new QProgressBar());

    QPushButton * annuler = new QPushButton("Cancel", this);
    memCancelToId.insert(annuler, id);
    memCancelClicked.insert(annuler, false);
    tableWidget->setCellWidget(ligneInsertion, C_ANNULER, annuler);
    connect(annuler, SIGNAL(clicked()), this, SLOT(Slot_CancelClicked()));

    QPushButton * resultat = new QPushButton("Results", this);
    memResultButtonToId.insert(resultat, id);
    resultat->setEnabled(false);
    tableWidget->setCellWidget(ligneInsertion, C_RESULTAT, resultat);
    connect(resultat, SIGNAL(clicked()), this, SLOT(Slot_ShowResults()));

    QPushButton * target = new QPushButton("Parameters");
    memTargetToId.insert(target, id);
    tableWidget->setCellWidget(ligneInsertion, C_PARAMS, target);
    connect(target, SIGNAL(clicked()), this, SLOT(Slot_ShowTarget()));

    memParams.insert(id, params);

    QJsonObject paramsObject = params.object();
    QTableWidgetItem * binaireItem = new QTableWidgetItem(paramsObject.find(CS_JSON_KEY_CALC_BIN).value().toString());
    binaireItem->setTextAlignment(Qt::AlignCenter);
    tableWidget->setItem(ligneInsertion, C_NOM, binaireItem);
}

void WidgetCalculs::Slot_StateUpdated(QUuid id, Calculation::Status state)
{
    const QMetaObject &mo = Calculation::staticMetaObject;
    int index = mo.indexOfEnumerator("State");
    QMetaEnum stateEnum = mo.enumerator(index);
    QString stateString = QString(stateEnum.valueToKey(state));

    LOG_DEBUG("Updating calculation state (id : " + id.toString() + ", row : "
              + QString::number(memIdToRow.value(id)) + ", state : " + stateString);

    QTableWidgetItem * stateTab = new QTableWidgetItem();
    stateTab->setText(stateString);
    stateTab->setTextAlignment(Qt::AlignCenter);
    tableWidget->setItem(memIdToRow.value(id), C_STATUT, stateTab);
}

void WidgetCalculs::Slot_ProgressUpdated(QUuid id, int value)
{
    LOG_DEBUG("Updating progress (QUid : " + id.toString() + ", progress : " + value  + ")");

    QProgressBar * progres = new QProgressBar(this);
    progres->setValue(value);
    tableWidget->setCellWidget(memIdToRow.value(id), C_PROGRES, progres);
}

void WidgetCalculs::Slot_CancelClicked()
{
    // Annulation si premier clic
    if(! memCancelClicked.value((QPushButton *) sender()))
    {
        MainWindowController::GetInstance()->CancelCalculation(memCancelToId.value((QPushButton *) sender()));
        changeCancelToDelete(memIdToRow.value(memCancelToId.value((QPushButton *) sender())));
    }
    // Suppression du calcul si second clic
    else
    {
        QUuid id = memCancelToId.value((QPushButton *) sender());
        int row = memIdToRow.value(id);
        QPushButton * cancelButton = (QPushButton *) sender();
        QPushButton * resultButton = (QPushButton *) tableWidget->cellWidget(row, C_RESULTAT);
        QPushButton * targetButton = (QPushButton *) tableWidget->cellWidget(row, C_PARAMS);

        tableWidget->removeRow(row);
        memIdToRow.remove(id);
        memCancelClicked.remove(cancelButton);
        memCancelToId.remove(cancelButton);
        memResultButtonToId.remove(resultButton);
        memTargetToId.remove(targetButton);
        memResults.remove(id);
        memParams.remove(id);

        for(int i=0; i<tableWidget->rowCount(); i++)
        {
            id = memCancelToId.value((QPushButton *) tableWidget->cellWidget(i, C_ANNULER));
            memIdToRow[id] = i;
        }
    }
}

void WidgetCalculs::Slot_CalculationDone(QUuid id, QJsonObject resultat)
{
    LOG_DEBUG("Receiving result for calculation (QUid : " + id.toString() + ")");
    memResults.insert(id, resultat);
    tableWidget->cellWidget(memIdToRow.value(id), C_RESULTAT)->setEnabled(true);
    changeCancelToDelete(memIdToRow.value(id));

}

void WidgetCalculs::Slot_ShowResults()
{
    QUuid id = memResultButtonToId.value((QPushButton *) sender());
    QJsonObject jsonObject(memResults.value(id));
    QJsonObject::iterator it = jsonObject.find(CS_JSON_KEY_FRAG_ID);
    if(it != jsonObject.end()) jsonObject.erase(it);

    QJsonDocument jsonDocument(jsonObject);
    QMessageBox * resultsMessage = new QMessageBox(this);
    resultsMessage->setText("Results for calculation " + id.toString());
    resultsMessage->setInformativeText(jsonDocument.toJson(QJsonDocument::Indented));
    resultsMessage->show();
}

void WidgetCalculs::changeCancelToDelete(int row)
{
    QPushButton * button = (QPushButton *) tableWidget->cellWidget(row, C_ANNULER);
    memCancelClicked[button] = true;
    button->setText("Delete");
}

void WidgetCalculs::Slot_ShowTarget()
{
    QUuid id = memTargetToId.value((QPushButton *) sender());
    QMessageBox * targetMessage = new QMessageBox(this);
    targetMessage->setText("Target for calculation " + id.toString());
    targetMessage->setText(memParams.value(id).toJson(QJsonDocument::Indented));
    targetMessage->show();
}