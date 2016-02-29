#ifndef CALCULATION_THREAD_H
#define CALCULATION_THREAD_H

#include <QObject>
#include <QJsonObject>

/**
 * @brief Cette classe représente le thread de calcul du client
 */
class CalculThread : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param parent : parent de l'objet
     */
    CalculThread();

    /**
     * @brief Destructeur de la classe
     */
    ~CalculThread();

public slots:
    /**
     * @brief Démarre le calcul
     * args   argument du calcul
     */
    void Slot_startCalcul(QJsonObject args);

    /**
     * @brief Arrête le calcul en cours
     */
    void Slot_stopCalcul();

signals:
    /**
     * @brief Emis quand le calcul est terminé
     * args   résultat du calcul
     */
    void sig_calculDone(QJsonObject args);

    /**
     * @brief Emis quand le calcul doit être annulé avant la fin
     */
    void sig_calculAborted();

private:
    bool _continueCalcul;
};


#endif // CALCULATION_THREAD_H
