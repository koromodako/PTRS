#ifndef ADDCALCULATIONWINDOW_H
#define ADDCALCULATIONWINDOW_H

#include <QMainWindow>
#include <QStringList>

/**
 * @brief Fenêtre permettant d'ajouter un calcul
 */
class AddCalculationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AddCalculationWindow(QWidget *parent = 0);

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

signals:

public slots:
    void Slot_pickedAnotherPlugin(QString name);
};

#endif // ADDCALCULATIONWINDOW_H
