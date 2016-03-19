#ifndef WIDGETCLIENTS_H
#define WIDGETCLIENTS_H

#include <QWidget>

/**
 * @brief Cette classe représente le widget affichant l'état des clients
 * (Clients disponibles, en cours de calcul, etc...)
 */
class WidgetClients : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetClients(QWidget *parent = 0);

signals:

public slots:
};

#endif // WIDGETCLIENTS_H
