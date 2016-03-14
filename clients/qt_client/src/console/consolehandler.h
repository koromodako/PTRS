#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <QThread>
#include <QTextStream>
#include <QMutex>
#include <QNetworkAddressEntry>
#include "src/const.h"
#include "src/calculation/calculation.h"

/**
 * @brief Cette classe gère les interactions avec l'utilisateur en console.
 *  Elle s'exécute dans un thread en parallèle de la boucle d'évènement principale de Qt.
 */
class ConsoleHandler : public QObject
{
    Q_OBJECT
public:
    ~ConsoleHandler(){}

    /**
     * @brief Defini le mutex à utiliser pour les entrées/sorties
     */
    void SetConsoleMutex(QMutex *mutex);

protected:
    /**
     * @brief Récupère l'instance unique de cette classe
     * @return
     */
    static ConsoleHandler & getInstance() { return _instance; }
    friend class ApplicationManager;

public slots:
    /**
     * Affiche le message de bienvenue et demande à l'utilisateur quoi faire
     */
    void Slot_init();

    /**
     * @brief Ce slot reçoit les réponse aux commandes envoyées en utilisant les signaux
     * @param command
     *      Type de commande ayant donné lieu à cette réponse
     * @param ok
     *      Statut de la réponse
     * @param message
     *      Message de réponse construit par l'émetteur de cette dernière
     */
    void Slot_response(Command command, bool ok, QString message);

signals:
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'état du client
     */
    void sig_state();
    /**
     * @brief Ce signal est émis lorsque l'utilisateur demande l'arrêt du client
     */
    void sig_shutdown();
    /**
     * @brief Ce signal est émis lorsque le thread de la console est sur le point de s'arrêter.
     */
    void sig_terminated();
    /**
     * @brief Ce signal est émis lorsque l'utilisateur demande une connexion au serveur.
     */
    void sig_connect();

private:
    /**
     * @brief Cette méthode affiche un message de bienvenue au démarrage du serveur
     */
    void welcome();
    /**
     * @brief Cette méthode affiche un prompt et réalise la lecture d'une commande
     * @param input
     *      La commande lue est placée dans cette variable
     * @return true si la commande lue n'est pas EOF
     */
    bool prompt(QString *input);
    /**
     * @brief Affiche une réponse à l'utilisateur
     * @param response
     *      Contenu de la réponse à afficher
     */
    void respond(QString response);
    /**
     * @brief Affiche une erreur à l'utilisateur (commande incorrecte, etc.)
     * @param error_str
     *      Message d'erreur
     * @param cmd
     *      Nom de la commande liée à l'erreur si il y a lieu
     */
    void error(QString errorStr, QString cmd = QString());
    /**
     * @brief Affiche un message dans la sortie standard (stdout)
     * @param output
     *      Message à afficher
     * @param endl
     *      Indique si la ligne doit être terminée ou non
     */
    void print(const QString & output, bool endl = true);
    /**
     * @brief Affiche l'aide demandée en fonction du contexte
     * @param cmd
     *      Permet d'afficher l'aide d'une commande en particulier
     */
    void help(const QString & cmd = QString());
    /**
     * @brief Affiche un message d'au revoir lors de la terminaison du serveur.
     */
    void goodbye();

    /**
     * @brief Interprète la commande passée en paramètre
     * @param input
     *      Commande saisie par l'utilisateur
     * @return true si l'interprétation est un succès (commande connue et paramètres présents)
     */
    bool interpret(QString & input);

private:
    ConsoleHandler();
    Q_DISABLE_COPY(ConsoleHandler)
    static ConsoleHandler _instance;

private:
    QTextStream _out;       // flux de sortie <=> std::cout
    QTextStream _in;        // flux de d'entrée <=> std::cin
    QMutex *_consoleMutex;
};

#endif // CONSOLEHANDLER_H
