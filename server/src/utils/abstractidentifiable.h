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
};

#endif // ABSTRACTIDENTIFIABLE_H
