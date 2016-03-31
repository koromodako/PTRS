#ifndef WIDGETCALCULS_H
#define WIDGETCALCULS_H

#include <QWidget>
#include "addcalculationwindow.h"

/**
 * @brief Cette classe représente le widget affichant l'état des calculs demandés
 */
class WidgetCalculs : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetCalculs(QWidget *parent = 0);

signals:

public slots:
    /**
     * @brief Slot déclenché par le bouton New Calculation, et ouvrant la fenêtre de
     * calcul si ce n'est pas déjà fait.
     */
    void Slot_newcalculation();

private:
    AddCalculationWindow *addCalcWindow;

};

#endif // WIDGETCALCULS_H
