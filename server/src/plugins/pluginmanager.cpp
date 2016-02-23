#include "pluginmanager.h"
#include "src/const.h"
#include "src/utils/logger.h"

#include <QFile>
#include <QProcess>

#define ENTRY_LIST_FILTER   QDir::NoFilter
#define ENTRY_LIST_SORT     QDir::Name
#define ENTRY_LIST() _plugins_dir.entryList(ENTRY_LIST_FILTER, ENTRY_LIST_SORT)

PluginManager PluginManager::_instance;

bool PluginManager::CheckPlugins() const
{
    bool ok = true;
    foreach (QString plugin, ENTRY_LIST())
    {   /// \todo do a MD5 checksum
    }
    return ok;
}

bool PluginManager::PluginExists(const QString &plugin_name) const
{
    return ENTRY_LIST().contains(plugin_name);
}

QStringList PluginManager::GetPluginsList() const
{
    return ENTRY_LIST();
}

bool PluginManager::RunPlugin(const QString &name, const QStringList &args, QString &out, QString &err) const
{
    bool ok = false;
    QProcess p;
    p.setProgram(QString("./%1/%2").arg(PLUGINS_DIR, name));
    p.setArguments(args);
    if(p.open(QIODevice::ReadOnly))
    {   p.waitForFinished();
        if(p.state() != QProcess::NotRunning)
        {   err.clear();
            out.clear();
            if(p.exitStatus() == QProcess::NormalExit)
            {   out.append(p.readAllStandardOutput());
            }
            else
            {   err.append(p.readAllStandardError());
            }
        }
    }
    return ok;
}

PluginManager::PluginManager()
{
    QDir dir;
    if(!dir.exists())
    {   if(!dir.mkdir(PLUGINS_DIR))
        {   LOG_CRITICAL("Can't make plugins directory !");
        }
    }
    _plugins_dir = QDir(PLUGINS_DIR);
}
