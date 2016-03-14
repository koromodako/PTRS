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
    enum Operation {
        SPLIT,
        JOIN,
        CALC
    };
    /**
     * @brief Construit une nouvelle instance de processus sur un calcul
     * @param calc
     *      Calcul auquel le processus est lié
     * @param op
     *      Opération réalisée par le plugin sur le calcul passé en paramètre
     */
    PluginProcess(Calculation * calc, Operation op, QObject * parent = NULL);
    ~PluginProcess(){} //do not delete calc here

private slots:
    /**
     * @brief Ce slot reçoit les notifications d'erreurs depuis le processus asynchrone
     *      effectuant l'operation demandée sur le plugin
     * @param error
     *      Code d'erreur renvoyé par le processus
     */
    void SLOT_ERROR(QProcess::ProcessError error);
    /**
     * @brief Ce slot reçoit les notifications de fin d'execution du plugin
     * @param exitCode
     *      Code de fin du processus
     * @param exitStatus
     *      Satut de fin du processus
     */
    void SLOT_FINISHED(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Calculation * _calculation;
    Operation _op;
    QString _out;
    QString _err;
};

typedef QList<PluginProcess*> PluginProcessList;

#endif // PluginProcess_H
