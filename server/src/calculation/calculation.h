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
    Q_OBJECT
public:
    /**
     * @brief Cette énumération décrit les différents états que peut prendre un calcul
     */
    enum State {
        BEING_SPLITTED,
        BEING_COMPUTED,
        BEING_JOINED,
        BEING_CANCELED,
        SCHEDULED,
        COMPLETED,
        COMPUTED,
        CANCELED,
        CRASHED
    };

    ~Calculation(){}

    /**
     * @brief Nombre de fragment pour le calcul
     * @return
     */
    inline int GetFragmentCount() const { return _fragments.count(); }
    /**
     * @brief Etat du calcul
     * @return
     */
    inline State GetStatus() const { return _state; }
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
    /**
     * @brief Donne la représentation JSON des fragments
     * @param format
     * @return
     */
    QString FragmentsToJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;
    /**
    * @brief Demande l'annulation du calcul
    */
    void Cancel();
    /**
     * @brief Cette méthode est appelée une fois le calcul fragmenté
     * @param json
     */
    void Splitted(QString json);
    /**
     * @brief Cette méthode est appelée une fois les résultats du calcul fusionnés
     * @param json
     */
    void Joined(QString json);
    /**
     * @brief Cette méthode est appelée une fois le calcul effectué
     * @param json
     */
    void Computed(QString json);
    /**
     * @brief Cette méthode est appelée quand le calcul à crashé
     * @param error
     */
    void Crashed(QString error);

signals:
    /**
     * @brief Ce signal est émis lorsque le calcul doit être annulé
     */
    void SIG_CANCELED();
    /**
     * @brief Ce signal est émis lorsque le calcul est prêt à être distribué
     */
    void SIG_SCHEDULED();
    /**
     * @brief Ce signal est émis lorsque le calcul est terminé (tous les fragments ont été fusionnés)
     */
    void SIG_COMPLETED();
    /**
     * @brief Ce signal est émis lorsque le calcul est terminé (dans le cas où le calcul est un fragment de calcul, coté client)
     */
    void SIG_COMPUTED();
    /**
     * @brief Ce signal est émis lorsque le plugin du calcul crash
     */
    void SIG_CRASHED();

private:
    // non instanciable autrement qu'en fabrique et non copiable
    Calculation(const QString &bin, const QVariantMap &params, QObject * parent = NULL);
    Q_DISABLE_COPY(Calculation)
    // attributs
    State _state;
    QString _bin;
    QVariantMap _params;
    QHash<QUuid,Calculation*> _fragments;
};

#endif // CALCULATION_H
