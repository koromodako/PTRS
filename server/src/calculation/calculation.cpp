#include "calculation.h"

void Calculation::AddFragment(CalculationFragment *fragment)
{   _fragments.insert(fragment->GetId(), fragment);
}

void Calculation::AddFragment(QList<CalculationFragment *> fragments)
{   foreach (CalculationFragment * calc_frag, fragments) {
        AddFragment(calc_frag);
    }
}

Calculation::Calculation(const QString & bin, const QVariantMap &params, QObject * parent) :
    AbstractIdentifiable(parent),
    _status(CALCS_SCHEDULED),
    _bin(bin),
    _params(params),
    _fragments()
{
}
