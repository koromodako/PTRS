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
    delete ui->mainToolBar;

    // ---- Onglet clients
    WidgetClients * clients = new WidgetClients();

    // ---- Onglet résultats
    WidgetCalculs * calculs = new WidgetCalculs();

    // ---- Configuration tabview
    QTabWidget * onglets = new QTabWidget(this);
    onglets->addTab(calculs, "Calculs");
    onglets->addTab(clients, "Clients");

    // ---- Configuration MainWindow
    this->setCentralWidget(onglets);
    this->setWindowTitle("PTRS - Serveur");
}

MainWindow::~MainWindow()
{
    delete ui;
}
