#include "pluginmanager.h"

#include "src/const.h"
#include <QFile>
#include <QProcess>

QDir PluginManager::_plugin_dir(PLUGINS_DIR);

bool PluginManager::CheckPlugins()
{
    bool ok = true;
    QDir dir;
    if(dir.exists())
    {   if(!dir.mkdir(PLUGINS_DIR))
        {   ok = false;
        }
    }
    foreach (QString plugin, _plugin_dir.entryList(QDir::Executable, QDir::Name))
    {   /// \todo do a MD5 checksum
    }
    return ok;
}

bool PluginManager::PluginExists(const QString &plugin_name)
{
    return _plugin_dir.entryList(QDir::Executable, QDir::Name).contains(plugin_name);
}

QStringList PluginManager::GetPluginsList()
{
    return _plugin_dir.entryList(QDir::Executable, QDir::Name);
}

bool PluginManager::RunPlugin(const QString &name, const QStringList &args, QString &out, QString &err)
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
}
