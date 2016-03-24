#include "widgetcalculs.h"
#include "addcalculationwindow.h"
#include <QtWidgets>

WidgetCalculs::WidgetCalculs(QWidget *parent) : QWidget(parent)
{

    // --- Layout
    QVBoxLayout *layout = new QVBoxLayout();

    // --- Initialisation du tableau
    QTableWidget * tableWidget = new QTableWidget(5, 6, this);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    tableWidget->verticalHeader()->setVisible(false);

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

    // Test bouton
    QPushButton * newCalc = new QPushButton("New Calculation");
    newCalc->setMaximumWidth(160);
    layout->addWidget(newCalc);
    layout->setAlignment(newCalc, Qt::AlignRight);

    connect(newCalc, QPushButton::clicked, this, Slot_newcalculation);

    // Bouton ajouter

    this->setLayout(layout);

}

void WidgetCalculs::Slot_newcalculation()
{
    AddCalculationWindow *newCalculation = new AddCalculationWindow(this);
    newCalculation->show();
}
