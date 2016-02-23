#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QStringList>
#include <QDir>

/**
 * @brief Cette classe gère les interactions avec les plugins
 */
class PluginManager
{
public:
    /**
     * @brief Vérification de l'intégrité des plugins
     * @return
     */
    static bool CheckPlugins();
    /**
     * @brief Vérification de l'existence d'un plugin
     * @param plugin_name
     *      Nom du plugin dont l'existence doit être vérifiée
     * @return
     */
    static bool PluginExists(const QString & plugin_name);
    /**
     * @brief Récupération de la liste des plugins
     * @return
     */
    static QStringList GetPluginsList();
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
    static bool RunPlugin(const QString & name, const QStringList & args, QString & out, QString & err);


private:
    PluginManager();
    Q_DISABLE_COPY(PluginManager)
    static QDir _plugin_dir;

};

#endif // PLUGINMANAGER_H
