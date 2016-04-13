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

    /**
     * @brief Slot déclenché lors du progrès d'un calcul
     */
    void Slot_ProgressUpdated(QUuid id, int value);

    /**
     * @brief Slot déclenché lors d'un appui sur le bouton d'annulation du calcul
     */
    void Slot_CancelClicked();

private:
    AddCalculationWindow *addCalcWindow;

    QTableWidget * tableWidget;

    /** Faute de meilleure solution **/
    QHash<QUuid, int> memIdToRow;
    QHash<QPushButton *, QUuid> memButtonToId;
    QHash<QPushButton *, bool> memButtonClicked;
    /** --------------------------- **/

    enum Colonnes {C_ID, C_NOM, C_STATUT, C_PROGRES, C_CLIENTS, C_ANNULER, C_RESULTAT};

};

#endif // WIDGETCALCULS_H
