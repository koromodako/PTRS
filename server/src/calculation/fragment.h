#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "../utils/abstractidentifiable.h"
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>

class Calculation;
class Fragment : public AbstractIdentifiable
{
    Q_OBJECT
public:
    /**
     * @brief Retourne le calcul parent
     * @return
     */
    const Calculation * GetCalculation() const;

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
    static Fragment *FromJson(Calculation *parent, const QByteArray &json, QString &errorStr);

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
    void Slot_computed(const QJsonObject &json);

    /**
     * @brief Ce slot met à jour l'avancement du calcul du fragment
     * @param int nouvel avancemnt entre [0 et 100]
     */
    void Slot_updateProgress(int progress);

signals:
    /**
     * @brief Ce signal est émis lorsque le calcul doit être annulé
     */
    void sig_canceled();
    /**
     * @brief Ce signal est émis quand l'avancement du fragment est mis à jour
     */
    void sig_progressUpdated(int oldValue, int newValue);


private:
    // non instanciable autrement qu'en fabrique et non copiable
    Fragment(const QString &bin, const QVariantMap &params, Calculation *parent = NULL);
    Q_DISABLE_COPY(Fragment)


    QString _bin;
    Calculation *_calculation;
    QVariantMap _params;
    int _progress;
    QJsonObject _result;
};

#endif // FRAGMENT_H
