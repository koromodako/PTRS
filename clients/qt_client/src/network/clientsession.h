#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <QThread>
#include <QTimer>
#include "src/network/etat/abstractstate.h"
#include "src/calculthread.h"
/**
 * @brief Cette classe représente une session client, c'est à dire une connexion client active.
 */
class ClientSession : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur par défault
     * @param associatedSocket : le socket de la connexion
     */
    ClientSession();

    /**
     * @brief Destructeur de la classe
     */
    ~ClientSession();

    /**
     * @brief Retourne l'id du fragment de calcul actuellement calculé par
     *        le client ou -1
     */
    inline int FragmentId() const;

    /**
     * @brief Retourne l'identifiant de ce client
     */
    QString Id() const;

    /**
     * @brief Envoie la commande donnée au Client
     * @param reqtype le type de la commande
     * @param args les arguments à transmettre au client
     */
    void SendCmd(ReqType reqtype, const QString &args);

    /**
     * @brief Change l'état courant
     */
    void SetCurrentState();

    /**
     * @brief Definie le nouvel identifiant de ce client
     */
    void setId(const QString &id);

public slots:
    /**
     * @brief Prévient le serveur que le calcul a annulé
     */
    void AbortCalcul();

    /**
     * @brief Envoie le résultat du calcul au serveur
     * @param args les résultats du calcul à transmettre au serveur
     */
    void SendResultToServer(QJsonObject args);

signals:
    /**
     * @brief Emis pour demander au thread de commencer le calcul
     */
    void requestCalculStart(QJsonObject args);

    /**
     * @brief Emis pour demander au thread d'arrêter le calcul
     */
    void requestCalculStop();

private:

    /**
     * @brief Envoie un message UDP pour récuperer l'ID d'un serveur
     */
    void findServer();

    /**
     * @brief Initialise l'automate et les états
     */
    void initializeStateMachine();

    /**
     * @brief récupère la réponse du serveur et se connecte avec celui-ci en TCP
     */
    void readBroadcastDatagram();

private slots:

    /**
     * @brief Passe l'automate en mode déconnecté
     */
    void disconnect();

    /**
     * @brief Traite la commande du type donné avec les arguments donnés
     * @param reqType le code de la requête récupéré
     * @param args les arguments de la requète
     */
    void processCmd(ReqType reqType, const QStringList &args);

    /**
     * @brief Traite la reception des messages TCP du client
     */
    void processReadyRead();

private:
    QTimer _broadcastTimer;
    QUdpSocket *_broadcastSocket;
    AbstractState *_currentState;
    AbstractState *_disconnectedState;
    int _fragmentId;
    QString _id;
    QTcpSocket *_socket;
    QThread *_thread;
    QMap<QObject *, AbstractState *> _transitionsMap;
    CalculThread* _worker;
};

inline int ClientSession::FragmentId() const
{
    return _fragmentId;
}

inline QString ClientSession::Id() const
{
    return _id;
}
#endif // CLIENT_SESSION_H
