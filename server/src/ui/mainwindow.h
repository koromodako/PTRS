#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    static MainWindow *GetInstance() { return _instance; }
    static MainWindow *CreateInstance() {
        _instance = new MainWindow;
        return _instance;
    }

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    /**
     * @brief Signal envoyé quand la fenêtre est fermée.
     */
    void Sig_close();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    Q_DISABLE_COPY(MainWindow)

    static MainWindow *_instance;
};

#endif // MAINWINDOW_H
