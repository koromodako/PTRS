#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>

#include <QByteArray>
#include <QUuid>
#include <QString>
#include <QJsonDocument>

#include "const.h"
#include "calculation/calculation.h"

class UserInterface : public QObject
{
    Q_OBJECT
public:
    explicit UserInterface(QObject *parent = 0);
    virtual ~UserInterface(){}

public slots:
    /**
     * Affiche le message de bienvenue et demande à l'utilisateur quoi faire
     */
    virtual void Slot_init() = 0;

    /**
     * @brief Ce slot reçoit les réponse aux commandes envoyées en utilisant les signaux
     * @param command
     *      Type de commande ayant donné lieu à cette réponse
     * @param ok
     *      Statut de la réponse
     * @param message
     *      Message de réponse construit par l'émetteur de cette dernière
     */
    virtual void Slot_response(Command command, bool ok, QString message) = 0;

    virtual void Slot_newClient(QUuid clientId);
    virtual void Slot_newCalculation(QUuid calculationId, QJsonDocument params);
    virtual void Slot_stateUpdated(QUuid id, Calculation::State state);


signals:
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'exécution d'un calcul
     * @param calculationOrderJSON
     *      Bloc JSON de description du calcul à segmenter et répartir
     */
    void sig_exec(QByteArray calculationOrderJSON);
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande le statut des calculs enregistrés
     */
    void sig_status();
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande les résultat d'un calcul
     * @param id
     *      Identifiant du calcul dont l'utilisateur souhaite obtenir le résultat
     */
    void sig_result(QUuid id, QString filename = QString());
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'annulation d'un calcul
     * @param id
     *      Identifiant du calcul dont l'utilisateur souhaite obtenir le résultat
     */
    void sig_cancel(QUuid id);
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'état du serveur
     */
    void sig_state();
    /**
     * @brief Ce signal est émis lorsque l'utilisateur demande l'arrêt du serveur
     */
    void sig_shutdown();
    /**
     * @brief Ce signal est émis lorsque le thread de la console est sur le point de s'arrêter.
     */
    void sig_terminated();


public slots:
};

#endif // USERINTERFACE_H
