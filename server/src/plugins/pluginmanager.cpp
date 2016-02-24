#include "pluginmanager.h"
#include "src/const.h"
#include "src/utils/logger.h"
#include "src/calculation/specs.h"

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

void PluginManager::Split(Calculation *calc)
{   // -- lancement du processus associé
    startProcess(calc, CalculationProcess::SPLIT);
}

void PluginManager::Join(Calculation *calc)
{   // -- lancement du processus associé
    startProcess(calc, CalculationProcess::JOIN);
}

void PluginManager::startProcess(Calculation * calc, CalculationProcess::Operation op)
{
    // -- création d'un nouveau processus
    CalculationProcess * cp = new CalculationProcess(calc, op);
    // -- set process program
    QString command = QString("%1/%2 ").arg(_plugins_dir.absolutePath(),calc->GetBin());
    // -- set process arguments
    switch (op) {
    case CalculationProcess::SPLIT:
        command.append(CS_OP_SPLIT).append(' ').append(calc->ToJson());
        break;
    case CalculationProcess::JOIN:
        command.append(CS_OP_JOIN).append(' ').append(calc->FragmentsToJson());
        break;
    case CalculationProcess::CALC:
        command.append(CS_OP_CALC).append(' ').append(calc->ToJson());
    default:
        LOG_CRITICAL("Processus started without arguments : unhandled operation is the cause !");
        break;
    }
    LOG_DEBUG(QString("Command is : '%1'").arg(command));
    // -- ajout du process à la liste
    _pending_processes.append(cp);
    // -- lancement du processus
    cp->start(command);
    cp->waitForFinished();
}

void PluginManager::SLOT_TERMINATE()
{   LOG_DEBUG("SLOT_TERMINATE() called.");
    // -- kill all pending processes
    while(!_pending_processes.isEmpty())
    {   CalculationProcess * cp = _pending_processes.takeFirst();
        cp->kill();
        cp->waitForFinished();
        delete cp;
    }
    // -- emit SIG_TERMINATED
    LOG_DEBUG("SIG_TERMINATED() emitted.");
    emit SIG_TERMINATED();
}

PluginManager::PluginManager() :
    _plugins_dir(),
    _pending_processes()
{
}
