#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../userinterface.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Cette classe représente la fenêtre principale de l'IHM serveur.
 * Elle permet de suivre et de gérer les calculs.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    Q_DISABLE_COPY(MainWindow)
};

#endif // MAINWINDOW_H
