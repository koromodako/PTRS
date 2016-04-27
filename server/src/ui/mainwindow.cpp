#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph/graphwidget.h"
#include "mainwindowcontroller.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ---- Configuration UI
    ui->setupUi(this);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0,15,0,0);
    centralWidget->setLayout(centralLayout);

    // ---- Onglet clients
    _graphWidget = new GraphWidget();

    // ---- Onglet résultats
    _calculWidget = new WidgetCalculs();

    // ---- Configuration tabview
    QTabWidget * onglets = new QTabWidget(this);
    onglets->addTab(_calculWidget, "Calculations");
    onglets->addTab(_graphWidget, "Clients");
    centralLayout->addWidget(onglets);

    // ---- Configuration MainWindow
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("PICARD - Server Control");
    this->setMinimumSize(900, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    MainWindowController::GetInstance()->windowClosed();
}

GraphWidget *MainWindow::getGraphWidget()
{
    return _graphWidget;
}

void MainWindow::closeAddCalculationWindow()
{
    _calculWidget->closeAddCalculationWindow();
}
