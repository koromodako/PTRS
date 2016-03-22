#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "../utils/abstractidentifiable.h"
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>

class Fragment : public AbstractIdentifiable
{
    Q_OBJECT
public:
    enum State {
        BEING_COMPUTED,
        BEING_CANCELED,
        SCHEDULED,
        COMPLETED,
        COMPUTED,
        CANCELED,
        CRASHED
    };
    /**
     * @brief Retourne le résultat du fragment
     * @return
     */
    inline const QJsonObject & GetResult() const { return _result; }

    /**
     * @brief Binaire utilisé pour les operations split, calc et join
     * @return
     */
    inline const QString & GetBin() const { return _bin; }

    /**
     * @brief Donne la représentation JSON du fragment
     * @param format
     * @return
     */
    QString ToJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;

    /**
     * @brief FromJson
     * @param parent
     * @param json
     * @param errorStr
     * @return
     */
    static Fragment *FromJson(QObject *parent, const QByteArray &json, QString &errorStr);

    /**
    * @brief Retourne la progression du calcul
    * @return un entier entre 0 et 100
    */
    inline int GetProgress() const { return _progress; }

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

    void Slot_updateProgress(int progress);

    /**
     * @brief Cette méthode est appelée quand le calcul commence
     */
    void Slot_started();

signals:
    /**
     * @brief Ce signal est émis lorsque le calcul est prêt à être distribué
     * @param fragment fragment que le client devra calculer
     */
    void sig_scheduled(const Fragment *fragment);
    /**
     * @brief Ce signal est émis lorsque le calcul doit être annulé
     */
    void sig_canceled();
    /**
     * @brief Ce signal est émis lorsque le calcul est terminé (dans le cas où le calcul est un fragment de calcul, coté client)
     */
    void sig_computed();
    /**
     * @brief Ce signal est émis lorsque le plugin du calcul crash
     */
    void sig_crashed();

    void sig_progressUpdated(QUuid idFragment, int value);
    /**
     * @brief Emis quand l'état d'un calcul est mis à jour
     * @param idCalculation l'id de ce calcul
     * @param state le nouvel état du calcul
     */
    void sig_stateUpdated(QUuid idCalculation, Fragment::State state);

private:
    void setCurrentState(Fragment::State state);

private:
    // non instanciable autrement qu'en fabrique et non copiable
    Fragment(const QString &bin, const QVariantMap &params, QObject *parent = NULL);
    Q_DISABLE_COPY(Fragment)


    QString _bin;
    QVariantMap _params;
    State _state;
    int _progress;
    QJsonObject _result;
};

#endif // FRAGMENT_H
