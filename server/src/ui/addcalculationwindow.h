#ifndef ADDCALCULATIONWINDOW_H
#define ADDCALCULATIONWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QMap>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QString>

#include "mainwindowcontroller.h"

/**
 * @brief Fenêtre permettant d'ajouter un calcul
 */
class AddCalculationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AddCalculationWindow(QWidget *parent = 0);
    /**
     * @brief Affiche la fenêtre en remettant tous les champs à zéro.
     * Ne fait rien si la fenêtre est déjà visible
     */
    void showAndReset();

private:
    /**
     * @brief Minimum et maximum des QSpinBox utilisées ici
     */
    const int SPIN_BOX_RANGE = 1000;
    /**
     * @brief Pas des QDoubleSpinBox
     */
    const double DOUBLE_STEP = 0.01;

    void updateOptions(QString selectedPlugin, QStringList itemNames = QStringList(),
                       QStringList itemTypes = QStringList());

    QMap<QString, QSpinBox *> intSelectors;
    QMap<QString, QDoubleSpinBox *> doubleSelectors;
    QMap<QString, QLineEdit *> stringSelectors;
    QString currentCalculationName;
    QWidget * widgetCalculs;

signals:

public slots:
    void Slot_pickedAnotherPlugin(QString name);
    void Slot_runCalculation();

private:
    void fetchPluginParameters(QString name);

    MainWindowController *mainWindowController;

};

#endif // ADDCALCULATIONWINDOW_H
