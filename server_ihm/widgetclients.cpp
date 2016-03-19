#include "widgetclients.h"
#include <QPushButton>
#include <QtWidgets>


WidgetClients::WidgetClients(QWidget *parent) : QWidget(parent)
{
    // Layout
    QVBoxLayout *layout = new QVBoxLayout;

    // Bouton test
    QPushButton * bouton = new QPushButton(this);
    bouton->setText("Bouton");
    layout->addWidget(bouton);

    this->setLayout(layout);
}
