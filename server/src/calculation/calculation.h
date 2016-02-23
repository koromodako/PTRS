#ifndef CALCULATION_H
#define CALCULATION_H

#include "../const.h"
#include "../utils/abstractidentifiable.h"
#include <QHash>
#include <QVariantMap>
#include <QJsonDocument>

/**
 * @brief Cette classe représente un calcul distribuable
 */
class Calculation : public AbstractIdentifiable
{
public:
    ~Calculation(){}

    /**
     * @brief Ajoute un fragment au calcul
     * @param fragment
     *      Fragment de calcul a ajouter
     */
    void AddFragment(Calculation * fragment);
    /**
     * @brief Ajoute une liste de fragments au calcul
     * @param fragments
     */
    void AddFragment(QList<Calculation*> fragments);

    /**
     * @brief Nombre de fragment pour le calcul
     * @return
     */
    inline int GetFragmentCount() const { return _fragments.count(); }
    /**
     * @brief Etat du calcul
     * @return
     */
    inline CalculationStatus GetStatus() const { return _status; }
    /**
     * @brief Binaire utilisé pour les operations split, calc et join
     * @return
     */
    inline QString GetBin() const { return _bin; }
    /**
     * @brief Paramètres nécessaires à l'execution du calcul
     * @return
     */
    inline QVariantMap GetParams() const { return _params; }

    /**
     * @brief Méthode de fabrique pour construire un calcul à partir de sa représentation JSON
     * @param parent
     * @param json
     * @param error_str
     * @return
     */
    static Calculation *FromJson(QObject * parent, const QByteArray & json, QString &error_str);
    /**
     * @brief Donne la représentation JSON du calcul
     * @param format
     * @return
     */
    QString ToJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;

#ifdef QT_DEBUG
    QString dump() const;
#endif

private:
    // non instanciable autrement qu'en fabrique et non copiable
    Calculation(const QString &bin, const QVariantMap &params, QObject * parent = NULL);
    Q_DISABLE_COPY(Calculation)
    // attributs
    CalculationStatus _status;
    QString _bin;
    QVariantMap _params;
    QHash<QUuid,Calculation*> _fragments;
};

#endif // CALCULATION_H
