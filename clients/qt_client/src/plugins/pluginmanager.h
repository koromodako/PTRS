#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "calculationprocess.h"
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
     * @brief Lance le calcul
     * @param calc
     */
    void Calc(Calculation * calc);

private:
    /**
     * @brief Démarre un nouveau processus pour un calcul
     * @param program
     * @param args
     */
    void startProcess(Calculation * calc, CalculationProcess::Operation op);

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
    friend class ReadyState;

    QDir _plugins_dir;
    CalculationProcessList _pending_processes;
};

#endif // PLUGINMANAGER_H
