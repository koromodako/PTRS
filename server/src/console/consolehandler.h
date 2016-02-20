#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <QThread>
#include <QTextStream>
#include "src/const.h"
#include "src/calculation/calculation.h"
#include "src/calculation/calculationorder.h"

/**
 * @brief Cette classe gère les interactions avec l'utilisateur en console.
 *  Elle s'exécute dans un thread en parallèle de la boucle d'évènement principale de Qt.
 */
class ConsoleHandler : public QThread
{
    Q_OBJECT
public:
    ~ConsoleHandler(){}
    /**
     * @brief Récupère l'instance unique de cette classe
     * @return
     */
    static ConsoleHandler & GetInstance() { return _instance; }


protected:
    /**
     * @override
     * @see QThread::run()
     */
    void run();

public slots:
    /**
     * @brief Ce slot reçoit les réponse aux commandes envoyées en utilisant les signaux
     * @param command
     *      Type de commande ayant donné lieu à cette réponse
     * @param ok
     *      Statut de la réponse
     * @param message
     *      Message de réponse construit par l'émetteur de cette dernière
     */
    void SLOT_RESPONSE(Command command, bool ok, QString message);

signals:
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'exécution d'un calcul
     * @param calculationOrderJSON
     *      Bloc JSON de description du calcul à segmenter et répartir
     */
    void SIG_EXEC(QByteArray calculationOrderJSON);
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande le statut des calculs enregistrés
     */
    void SIG_STATUS();
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande les résultat d'un calcul
     * @param id
     *      Identifiant du calcul dont l'utilisateur souhaite obtenir le résultat
     */
    void SIG_RESULT(QUuid id, QString filename = QString());
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'annulation d'un calcul
     * @param id
     *      Identifiant du calcul dont l'utilisateur souhaite obtenir le résultat
     */
    void SIG_CANCEL(QUuid id);
    /**
     * @brief Ce signal est émis chaque fois que l'utilisateur demande l'état du serveur
     */
    void SIG_STATE();
    /**
     * @brief Ce signal est émis lorsque l'utilisateur demande l'arrêt du serveur
     */
    void SIG_SHUTDOWN();
    /**
     * @brief Ce signal est émis lorsque le thread de la console est sur le point de s'arrêter.
     */
    void SIG_TERMINATED();

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
    void error(QString error_str, QString cmd = QString());
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
};

#endif // CONSOLEHANDLER_H
