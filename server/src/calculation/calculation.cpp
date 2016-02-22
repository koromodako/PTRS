#include "calculation.h"

Calculation::Calculation(const QString & bin, QObject * parent) :
    AbstractIdentifiable(parent),
    _status(CALCS_SCHEDULED),
    _bin(bin),
    _fragments()
{
}
