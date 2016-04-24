#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <QThread>
#include <QTimer>
#include <QJsonObject>
#include "src/network/etat/abstractstate.h"
#include "src/plugins/pluginprocess.h"

/// Ce type est celui utilisé pour stocker la taille d'un message et taille maximale associée
typedef quint32 msg_size_t;
#define MSG_SIZE_MAX UINT32_MAX

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
    void Send(ReqType reqtype, const QString &content = "");

    /**
     * @brief Change l'état courant
     */
    void SetCurrentState();

    /**
     * @brief Definie le nouvel identifiant de ce client
     */
    void setId(const QString &id);

    /**
     * @brief Retourne le calcul en cours
     * @return
     */
    inline Calculation* GetCurrentCalculation() { return _currentCalculation; }

public slots:
    /**
     * @brief Prévient le serveur que le calcul a annulé
     */
    void Slot_abortCalcul();

    /**
     * @brief Envoie le résultat du calcul au serveur
     */
    void Slot_sendResultToServer();

    /**
     * @brief Démarre le calcul donné si aucun n'est déjà en cours
     */
    void Slot_startCalculation(Calculation *calculation);

signals:
    /**
     * @brief Emis pour demander au thread de commencer le calcul
     */
    void sig_requestCalculStart(Calculation *calculation);

    /**
     * @brief Emis pour demander au thread d'arrêter le calcul
     */
    void sig_requestCalculStop();

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
    void slot_disconnect();

    /**
     * @brief Traite la commande du type donné avec les arguments donnés
     * @param reqType le code de la requête récupéré
     * @param args les arguments de la requète
     */
    void slot_processRequest(ReqType reqType, const QByteArray &content);

    /**
     * @brief Traite la reception des messages TCP du client
     */
    void slot_processReadyRead();

private:
    QTimer _broadcastTimer;
    QUdpSocket *_broadcastSocket;
    Calculation *_currentCalculation;
    AbstractState *_currentState;
    AbstractState *_disconnectedState;
    int _fragmentId;
    QString _id;
    QTcpSocket *_socket;
    QMap<QObject *, AbstractState *> _transitionsMap;
    msg_size_t _blockSize;
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
