#ifndef CALCULATION_H
#define CALCULATION_H

#include "fragment.h"
#include "../const.h"
#include <QHash>

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
        SCHEDULED,
        BEING_COMPUTED,
        COMPUTED,
        BEING_JOINED,
        COMPLETED,
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

    /**
     * @brief Ce slot met à jour l'avancement d'un calcul
     * @param le nouvel avancement du calcul
     */
    void Slot_updateProgress(int progression);

signals:
    /**
     * @brief Emis quand un calcul est terminé
     * @param idCalculation l'id du calcul
     * @param result le résultat du calcul
     */
    void sig_calculationDone(QUuid idCalculation, const QJsonObject &result);

    /**
     * @brief Emis quand l'avancement d'un calcul est mis à jour
     * @param idCalculation l'id de ce calcul
     * @param value Avancement entre 0 et 100
     */
    void sig_scheduled(const Fragment *fragment);

    /**
     * @brief Emis quand l'état d'un calcul est mis à jour
     * @param idCalculation l'id de ce calcul
     * @param state le nouvel état du calcul
     */
    void sig_stateUpdated(QUuid idCalculation, Calculation::State state);

    /**
     * @brief Ce signal est émis lorsque le calcul doit être annulé
     */
    void sig_canceled();

    void sig_progressUpdated(QUuid idFragment, int value);

private:
    /**
     * @brief Modifie l'état actuel
     */
    void setCurrentState(Calculation::State state);

    // non instanciable autrement qu'en fabrique et non copiable
    Calculation(const QString &bin, const QVariantMap &params, QObject * parent = NULL);
    Q_DISABLE_COPY(Calculation)

private slots:
    /**
     * @brief Mais à jour la progression du calcul quand la progression d'un des fragments a évolué
     */
    void slot_updateChildrenProgress();

private:

    // attributs
    State _state;
    QString _bin;
    QVariantMap _params;
    QHash<QUuid,Fragment*> _fragments;
    int _progress;
    QJsonObject _result;
};
#endif // CALCULATION_H
