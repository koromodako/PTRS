#ifndef WIDGETCALCULS_H
#define WIDGETCALCULS_H

#include <QWidget>
#include <QtWidgets>
#include "addcalculationwindow.h"
#include "../calculation/calculation.h"

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

    /**
     * @brief Slot déclenché lors de la mise à jour de l'état d'un calcul
     */
    void Slot_StateUpdated(QUuid id, Calculation::State state);

private:
    AddCalculationWindow *addCalcWindow;

    QTableWidget * tableWidget;
    QHash<QUuid, int> memorisationPositions; // Permet de faire la correspondance entre un QUuid et une ligne

};

#endif // WIDGETCALCULS_H
