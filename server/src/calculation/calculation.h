#ifndef CALCULATION_H
#define CALCULATION_H

#include "../const.h"
#include "../utils/abstractidentifiable.h"
#include <QHash>
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>

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
    inline const QString & GetBin() const { return _bin; }

    /**
     * @brief Paramètres nécessaires à l'execution du calcul
     * @return
     */
    inline const QVariantMap & GetParams() const { return _params; }

    /**
     * @brief Retourne le résultat du calcul
     * @return
     */
    inline const QJsonObject & GetResult() const { return _result; }

    /**
     * @brief Méthode de fabrique pour construire un calcul à partir de sa représentation JSON
     * @param parent
     * @param json
     * @param error_str
     * @return
     */
    static Calculation *FromJson(QObject * parent, const QByteArray & json, QString &errorStr);

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
    QString FragmentsResultsToJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;

    /**
    * @brief Demande l'annulation du calcul
    */
    void Cancel();

    /**
     * @brief Cette méthode est appelée une fois le calcul fragmenté
     * @param json
     */
    void Splitted(const QByteArray &json);

    /**
     * @brief Cette méthode est appelée une fois les résultats du calcul fusionnés
     * @param json
     */
    void Joined(const QByteArray &json);

public slots:
    /**
     * @brief Ce slot est appelée une fois le calcul effectué
     * @param json résultat en provenance du client
     */
    void Slot_computed(const QByteArray &json);

    /**
     * @brief Ce slot est appelée quand le calcul à crashé
     * @param error message d'erreur
     */
    void Slot_crashed(QString error);

signals:
    /**
     * @brief Ce signal est émis lorsque le calcul doit être annulé
     */
    void sig_canceled();

    /**
     * @brief Ce signal est émis lorsque le calcul est prêt à être distribué
     * @param args argument à fournir au client pour démarrer son calcul
     * @param fragment fragment que le client devra calculer
     */
    void sig_scheduled(const QString &args, const Calculation *fragment);

    /**
     * @brief Ce signal est émis lorsque le calcul est terminé (tous les fragments ont été fusionnés)
     */
    void sig_completed();

    /**
     * @brief Ce signal est émis lorsque le calcul est terminé (dans le cas où le calcul est un fragment de calcul, coté client)
     */
    void sig_computed();

    /**
     * @brief Ce signal est émis lorsque le plugin du calcul crash
     */
    void sig_crashed();

private:
    // non instanciable autrement qu'en fabrique et non copiable
    Calculation(const QString &bin, const QVariantMap &params, QObject * parent = NULL);
    Q_DISABLE_COPY(Calculation)

    // attributs
    State _state;
    QString _bin;
    QVariantMap _params;
    QHash<QUuid,Calculation*> _fragments;
    QJsonObject _result;
};

#endif // CALCULATION_H
