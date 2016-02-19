#include "abstractidentifiable.h"

AbstractIdentifiable::AbstractIdentifiable(QObject * parent) :
    QObject(parent),
    _id(QUuid::createUuid())
{

}
