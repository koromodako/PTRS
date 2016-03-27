#include "addcalculationwindow.h"
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>

AddCalculationWindow::AddCalculationWindow(QWidget *parent) : QMainWindow(parent)
{

    this->updateOptions("bruteforce",
                        QStringList() << "Un" << "Deux" << "Trois",
                        QStringList() << "int" << "string" << "double");

    this->setWindowTitle("PTRS - New calculation");
}

void AddCalculationWindow::updateOptions(QString selectedPlugin, QStringList itemNames, QStringList itemTypes)
{
    QString pluginNames [] = { "bruteforce", "mergesort" };
    int pluginCount = 2;

    //liste de paramètres pour le plugin actuel
    QWidget *listParameters = new QWidget(this);
    QGridLayout *listLayout = new QGridLayout(listParameters);

    int pluginIndex = 0;

    QComboBox *pluginList = new QComboBox(listParameters);
    for(int i = 0; i < pluginCount; i++)
    {
        pluginList->insertItem(i, pluginNames[i]);
        if(pluginNames[i] == selectedPlugin)
        {
            pluginIndex = i;
        }
    }
    pluginList->setCurrentIndex(pluginIndex);

    QLabel *pluginTitle = new QLabel("Use plugin: ", listParameters);

    listLayout->addWidget(pluginTitle, 0, 0);
    listLayout->addWidget(pluginList, 0, 1);

    connect(pluginList, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(Slot_pickedAnotherPlugin(QString)));

    QFrame* line = new QFrame(listParameters);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    listLayout->addWidget(line, 1, 0, 1, -1);

    listParameters->setLayout(listLayout);

    for(int i = 0; i < itemNames.size(); i++)
    {
        QLabel *name = new QLabel(itemNames[i], listParameters);
        QWidget *value = NULL;
        if(itemTypes[i] == "int")
        {
            QSpinBox *spin = new QSpinBox(listParameters);
            spin->setRange(-SPIN_BOX_RANGE, SPIN_BOX_RANGE);
            value = spin;
        }
        else if(itemTypes[i] == "double")
        {
            QDoubleSpinBox *spin = new QDoubleSpinBox(listParameters);
            spin->setRange(-SPIN_BOX_RANGE, SPIN_BOX_RANGE);
            spin->setSingleStep(DOUBLE_STEP);
            value = spin;
        }
        else
        {
            value = new QLineEdit(listParameters);
        }

        listLayout->addWidget(name, i+2, 0);
        listLayout->addWidget(value, i+2, 1);
    }

    QPushButton *calculate = new QPushButton("Calculate", listParameters);
    listLayout->addWidget(calculate, 2 + itemNames.size(), 0, 1, -1);

    this->setCentralWidget(listParameters);
}

void AddCalculationWindow::Slot_pickedAnotherPlugin(QString name)
{
    if(name == "bruteforce")
    {
        this->updateOptions(name,
                            QStringList() << "Un" << "Deux" << "Trois",
                            QStringList() << "int" << "string" << "double");
    }
    else if(name == "mergesort")
    {
        this->updateOptions(name,
                            QStringList() << "list",
                            QStringList() << "string");
    }
}
