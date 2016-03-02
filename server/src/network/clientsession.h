#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <QTcpSocket>
#include "src/network/etat/abstractstate.h"
#include "src/utils/abstractidentifiable.h"
#include "../calculation/calculation.h"
/**
 * @brief Cette classe représente une session client, c'est à dire une connexion client active.
 */
class ClientSession : public AbstractIdentifiable
{
    Q_OBJECT

    friend class AbstractState;
    friend class DisconnectedState;
    friend class ReadyState;
    friend class WaitingState;
    friend class WorkingAboutToStartState;
    friend class WorkingState;

public:
    /**
     * @brief Constructeur par défault
     * @param associatedSocket : le socket de la connexion
     * @param parent : parent de l'objet
     */
    ClientSession(QTcpSocket *associatedSocket, QObject *parent);

    /**
     * @brief Destructeur de la classe
     */
    ~ClientSession();

    /**
     * @brief Retourne un pointeur vers le calcul actuellement fait par
     *        le client ou NULL
     */
    inline const Calculation *Fragment() const;

    /**
     * @brief Demande au client de démarrer le calcul donné
     * @param fragmentId l'id du fragment de calcul (utile pour stopper
     *                   éventuellement le calcul)
     * @param args les arguments de calcul à transmettre au client
     * @return true si le calcul à pu démarrer, false sinon (si un calcul est déja en cours)
     */
    bool StartCalcul(const Calculation *fragment, const QString &json);

public slots:
    /**
     * @brief Arrête le calcul en cours s'il y en a un
     */
    void Slot_stopCalcul();

signals:
    /**
     * @brief Emit quand le client a arrété son calcul prématurément
     * @param errorMsg le message d'erreur sous forme de string
     */
    void sig_calculAborted(const QString &errorMsg);

    /**
     * @brief Emit quand un fragment à fini avec succès son calcul
     * @param json les résultat du calcul transmis par le client
     */
    void sig_calculDone(const QString &json);

    /**
     * @brief Emis quand le client est prêt à recevoir un nouveau calcul.
     * @param client Pointeur vers cette instance
     */
    void sig_ready(ClientSession *client);

    /**
     * @brief Emis quand le client commence à faire un calcul.
     * @param client Pointeur vers cette instance
     */
    void sig_working(ClientSession *client);

private:
    /**
     * @brief Initialise l'automate et les états
     */
    void initializeStateMachine();

    /**
     * @brief Envoie la commande donnée au Client
     * @param reqtype le type de la commande
     * @param args les arguments à transmettre au client
     */
    void sendCmd(ReqType reqtype, const QString &args);

    /**
     * @brief Effectue la transition de l'automate avec la liste des transitions données
     */
    void setCurrentState(const QMap<QObject *, AbstractState *> &transitionsMap);

    /**
     * @brief Change l'état courant et récupère le code d'erreur
     * @param errorCode l'erreur émise par l'état courant qui a déclenché le changement
     */
    void setCurrentStateAfterError(ErrorCode errorCode);

    /**
     * @brief Change l'état courant
     */
    void setCurrentStateAfterSuccess();

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
    void slot_processCmd(ReqType reqType, const QStringList &args);

    /**
     * @brief Traite la reception des messages TCP du client
     */
    void slot_processReadyRead();

private:
    AbstractState *_disconnectedState;
    AbstractState *_currentState;
    QMap<QObject *, AbstractState *> _doneTransitionsMap;
    QMap<QObject *, AbstractState *> _errorTransitionsMap;
    const Calculation *_fragment;
    QTcpSocket *_socket;
};

inline const Calculation *ClientSession::Fragment() const
{
    return _fragment;
}

#endif // CLIENT_SESSION_H
