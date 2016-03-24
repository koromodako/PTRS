#ifndef WIDGETCALCULS_H
#define WIDGETCALCULS_H

#include <QWidget>

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
     * @brief Slot déclenché par le bouton New Calculation
     */
    void Slot_newcalculation();

};

#endif // WIDGETCALCULS_H
