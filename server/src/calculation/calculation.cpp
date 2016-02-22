#include "calculation.h"

Calculation::Calculation(const QString & bin, QObject * parent) :
    AbstractIdentifiable(parent),
    _bin(bin),
    _fragments()
{
}
