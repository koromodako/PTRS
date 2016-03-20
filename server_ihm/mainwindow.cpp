#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgetclients.h"
#include "widgetcalculs.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ---- Configuration UI
    ui->setupUi(this);
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->setContentsMargins(0,15,0,0);
    centralWidget->setLayout(centralLayout);

    // ---- Onglet clients
    WidgetClients * clients = new WidgetClients();

    // ---- Onglet résultats
    WidgetCalculs * calculs = new WidgetCalculs();

    // ---- Configuration tabview
    QTabWidget * onglets = new QTabWidget(this);
    onglets->addTab(calculs, "Calculations");
    onglets->addTab(clients, "Clients");
    centralLayout->addWidget(onglets);

    // ---- Configuration MainWindow
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("PTRS - Server");
    this->setMinimumSize(900, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
}
