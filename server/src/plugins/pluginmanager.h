#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QStringList>
#include <QDir>

/**
 * @brief Cette classe gère les interactions avec les plugins
 */
class PluginManager : public QObject
{
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
    bool PluginExists(const QString & plugin_name) const;
    /**
     * @brief Récupération de la liste des plugins
     * @return
     */
    QStringList GetPluginsList() const;
    /**
     * @brief Execution du plugin <name> avec les arguments <args> et récupération
     *      de la sortie standard dans <out> et de la sortie d'erreur dans <err>
     * @param name
     *      Nom du plugin à exécuter
     * @param args
     *      Arguments en entrée du plugin
     * @param out
     *      Contenu de la sortie standard
     * @param err
     *      Contenu de la sortie d'erreur
     * @return
     */
    bool RunPlugin(const QString & name, const QStringList & args, QString & out, QString & err) const;


private: // singleton
    PluginManager();
    Q_DISABLE_COPY(PluginManager)
    static PluginManager _instance;
    static PluginManager & getInstance() { return _instance; }
    friend class ApplicationManager;
    friend class CalculationManager;

    QDir _plugins_dir;
};

#endif // PLUGINMANAGER_H
