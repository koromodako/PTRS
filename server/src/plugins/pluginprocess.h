#ifndef PluginProcess_H
#define PluginProcess_H

#include "src/calculation/calculation.h"
#include <QProcess>

class PluginProcess : public QProcess
{
    Q_OBJECT
public:
    /**
     * @brief Cette énumération définit les différentes opération qui peuvent être effectuées
     *          sur un calcul par un plugin
     */
    enum CalculationOperation {
        SPLIT,  ///< Opération de fragmentation d'un calcul
        JOIN,   ///< Opération d'aggrégation des résultats
        UI      ///< Opération de récupération de la description de l'interface utilisateur
    };
    /**
     * @brief Construit une nouvelle instance de processus sur un calcul
     * @param calc
     *      Calcul auquel le processus est lié
     * @param op
     *      Opération réalisée par le plugin sur le calcul passé en paramètre
     */
    PluginProcess(QString absExecDir, Calculation * calc, CalculationOperation op, QObject * parent = NULL);
    PluginProcess(QString absExecDir, Fragment *calc, QObject *parent = NULL);
    ~PluginProcess(){} //do not delete calc here
    /**
     * @brief Démarre l'exécution du plugin
     * @return retourne faux si aucun n'interpréteur n'a été trouvé pour le type script, sinon retourne toujours vrai
     */
    bool Start();

private slots:
    /**
     * @brief Ce slot reçoit les notifications d'erreurs depuis le processus asynchrone
     *      effectuant l'operation demandée sur le plugin
     * @param error
     *      Code d'erreur renvoyé par le processus
     */
    void Slot_error(QProcess::ProcessError error);
    /**
     * @brief Ce slot reçoit les notifications de fin d'execution du plugin pour un calcul
     * @param exitCode
     *      Code de fin du processus
     * @param exitStatus
     *      Satut de fin du processus
     */
    void Slot_calcFinished(int exitCode, QProcess::ExitStatus exitStatus);
    /**
     * @brief Ce slot reçoit les notifications de fin d'execution du plugin pour un fragment
     * @param exitCode
     *      Code de fin du processus
     * @param exitStatus
     *      Satut de fin du processus
     */
    void Slot_fragFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    /**
     * @brief Cette énumération définit les différents types de plugins supportés
     *      Le type inconnu n'existe pas, le type par défault utilisé est BINARY
     */
    enum Type {
        BINARY,     ///< Binaire compilé
        JAR,        ///< Archive JAR java
        SCRIPT      ///< Fichier script avec interpréteur sans #!/bin/... sinon considéré comme binaire
    };
    /**
     * @brief Détecte le type de plugin (binaire compilé, JAR, script)
     *          Pour l'instant on se contente de regarder l'extension
     * @return le type de plugin
     */
    Type detectType();
    QString selectInterpreter();

    QString _absExecDir;
    Calculation * _calculation;
    Fragment * _fragment;
    CalculationOperation _op;
    QString _out;
    QString _err;
};

typedef QList<PluginProcess*> PluginProcessList;

#endif // PluginProcess_H
