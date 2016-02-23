#ifndef CALCULATION_H
#define CALCULATION_H

#include "src/const.h"
#include "src/utils/abstractidentifiable.h"
#include "calculationfragment.h"
#include <QHash>

/**
 * @brief Cette classe représente un calcul distribuable
 */
class Calculation : public AbstractIdentifiable
{
public:
    ~Calculation(){}

    void AddFragment(CalculationFragment * fragment);
    void AddFragment(QList<CalculationFragment*> fragments);

    inline int GetFragmentCount() const { return _fragments.count(); }
    inline CalculationStatus GetStatus() const { return _status; }

protected:
    Calculation(const QString &bin, QObject * parent = NULL);
    friend class CalculationFactory; // seule la CalculationFactory est autorisée à créer des instances de cette classe

private:
    Q_DISABLE_COPY(Calculation)
    CalculationStatus _status;
    QString _bin;
    QHash<QUuid,CalculationFragment*> _fragments;
};

#endif // CALCULATION_H
