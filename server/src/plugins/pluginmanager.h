#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginprocess.h"
#include <QStringList>
#include <QDir>

/**
 * @brief Cette classe gère les interactions avec les plugins
 */
class PluginManager : public QObject
{
    Q_OBJECT
public:   
    /**
     * @brief Initiatlise le plugin manager en récupérant le chemin d'execution
     */
    bool Init();
    /**
     * @brief Vérification de l'intégrité des plugins
     * @return
     */
    bool CheckPlugins() const;
    /**
     * @brief Vérification de l'existence d'un plugin
     * @param plugin_name
     *      Nom du plugin dont l'existence doit être vérifiée
     * @return
     */
    bool PluginExists(const QString & pluginName) const;
    /**
     * @brief Récupération de la liste des plugins
     * @return
     */
    QStringList GetPluginsList() const;
    /**
     * @brief Lance le processus de fragmentation pour le calcul passé en paramètre
     * @param calc
     */
    void Split(Calculation * calc);
    /**
     * @brief Lance la procédure de fusion des résultats pour le calcul passé en paramètre
     * @param calc
     */
    void Join(Calculation * calc);
    /**
     * @brief Lance la procédure de récupération de l'interface utilisateur
     * @param calc
     */
    void Ui(Calculation * calc);
    /**
     * @brief Retourne le fichier sous forme d'un tableau d'octets pouvant être écrit dans le socket
     * @param arch
     *      Architecture du client
     * @param os
     *      OS du client
     * @param bin
     *      Nom du binaire
     * @warning la méthode appelante est chargée de libérer le tableau d'octets
     * @return
     */
    const QByteArray * GetPluginData(const QString &arch, const QString &os, QString bin);

private:
    /**
     * @brief Démarre un nouveau processus pour un calcul
     * @param program
     * @param args
     */
    void startCalcProcess(Calculation * calc, PluginProcess::CalculationOperation op);

signals:
    /**
     * @brief Ce signal est émis dès que Slot_terminate() a terminé ses traitements
     * @see Slot_terminate()
     */
    void sig_terminated();

public slots:
    /**
     * @brief Ce slot est appelé par l'application manager quand celui-ci souhaite terminer l'application.
     *          Il émet le signal sig_terminated() dès qu'il a terminé ses traitements
     * @see sig_terminated()
     */
    void Slot_terminate();


private: // singleton
    PluginManager();
    Q_DISABLE_COPY(PluginManager)
    static PluginManager _instance;
    static PluginManager & getInstance() { return _instance; }
    friend class ApplicationManager;
    friend class CalculationManager;
    friend class Calculation;
    friend class WorkingAboutToStartState;

    QDir _plugins_dir;
    PluginProcessList _processes;
};

#endif // PLUGINMANAGER_H
