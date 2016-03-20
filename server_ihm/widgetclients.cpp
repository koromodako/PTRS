#include "widgetclients.h"
#include <QPushButton>
#include <QtWidgets>


WidgetClients::WidgetClients(QWidget *parent) : QWidget(parent)
{
    // Layout
    QVBoxLayout *layout = new QVBoxLayout;

    this->setLayout(layout);
}
