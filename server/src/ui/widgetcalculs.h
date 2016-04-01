#ifndef WIDGETCALCULS_H
#define WIDGETCALCULS_H

#include <QWidget>
#include <QtWidgets>
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
    void Slot_AddNewCalculation();

    /**
     * @brief Slot déclenché lorsqu'un nouveau calcul est crée
     */
    void Slot_NewCalculation(QUuid id);

private:
    AddCalculationWindow *addCalcWindow;

    QTableWidget * tableWidget;

};

#endif // WIDGETCALCULS_H
