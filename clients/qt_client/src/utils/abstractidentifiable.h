#ifndef ABSTRACTIDENTIFIABLE_H
#define ABSTRACTIDENTIFIABLE_H

#include <QObject>
#include <QUuid>

/**
 * @brief Cette classe abstraite représente n'importe quel objet devant pouvoir être
 *      identifié de manière unique durant toute sa durée de vie
 */
class AbstractIdentifiable : public QObject
{
    friend class Calculation;

public:
    /**
     * @brief Retourne l'identifiant unique associé à l'objet
     * @return
     */
    QUuid GetId() const { return _id; }

protected:
    ~AbstractIdentifiable(){}
    AbstractIdentifiable(QObject *parent);

private:
    QUuid _id; // identifiant

    void SetId(QUuid id) { _id = id; }
};

#endif // ABSTRACTIDENTIFIABLE_H
