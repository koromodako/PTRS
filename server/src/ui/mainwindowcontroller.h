#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>

#include "../userinterface.h"
#include "mainwindow.h"

class MainWindowController : public UserInterface
{
    Q_OBJECT

public:
    MainWindowController(QObject *parent = 0);

    static MainWindowController *GetInstance() { return _instance; }
    static MainWindowController *CreateInstance() {
        _instance = new MainWindowController;
        return _instance;
    }

    virtual ~MainWindowController(){}

public slots:
    /**
     * Affiche le message de bienvenue et demande à l'utilisateur quoi faire
     */
    void Slot_init();

    /**
     * @brief Ce slot reçoit les réponse aux commandes envoyées en utilisant les signaux
     * @param command
     *      Type de commande ayant donné lieu à cette réponse
     * @param ok
     *      Statut de la réponse
     * @param message
     *      Message de réponse construit par l'émetteur de cette dernière
     */
    void Slot_response(Command command, bool ok, QString message);

private:
    MainWindow window;

    static MainWindowController *_instance;

    friend class MainWindow;

    void windowClosed();
};

#endif // MAINWINDOWCONTROLLER_H
