#include "pluginmanager.h"
#include "src/const.h"
#include "src/utils/logger.h"

#include <QCoreApplication>
#include <QFile>
#include <QProcess>

#define ENTRY_LIST_FILTER   QDir::Files | QDir::Executable
#define ENTRY_LIST_SORT     QDir::Name
#define ENTRY_LIST() _plugins_dir.entryList(ENTRY_LIST_FILTER, ENTRY_LIST_SORT)

PluginManager PluginManager::_instance;

bool PluginManager::Init()
{
    bool ok = false;
    _plugins_dir.cd(qApp->applicationDirPath());
    if(!_plugins_dir.cd(PLUGINS_DIR))
    {   if(!_plugins_dir.mkdir(PLUGINS_DIR))
        {   LOG_CRITICAL("Can't make plugins directory !");
        }
        if(!_plugins_dir.cd(PLUGINS_DIR))
        {   LOG_CRITICAL(QString("Directory '%1' doesn't exists !").arg(_plugins_dir.absolutePath()));
        }
        else {ok = true;}
    }
    else {ok = true;}
    if(ok){ LOG_INFO("Plugin dir found !"); }
    return ok;
}

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

PluginManager::PluginManager() :
    _plugins_dir()
{
}
