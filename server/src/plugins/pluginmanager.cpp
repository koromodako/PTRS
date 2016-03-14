#include "pluginmanager.h"
#include "src/const.h"
#include "src/utils/logger.h"
#include "src/calculation/specs.h"

#include <QCoreApplication>
#include <QFile>
#include <QUrl>

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

bool PluginManager::PluginExists(const QString &pluginName) const
{
    return ENTRY_LIST().contains(pluginName);
}

QStringList PluginManager::GetPluginsList() const
{
    return ENTRY_LIST();
}

void PluginManager::Split(Calculation *calc)
{   // -- lancement du processus associé
    startProcess(calc, PluginProcess::SPLIT);
}

void PluginManager::Join(Calculation *calc)
{   // -- lancement du processus associé
    startProcess(calc, PluginProcess::JOIN);
}

void PluginManager::startProcess(Calculation * calc, PluginProcess::Operation op)
{
    // -- création d'un nouveau processus
    PluginProcess * cp = new PluginProcess(calc, op);
    // -- set process program
    QString command = QString("%1/%2 ").arg(_plugins_dir.absolutePath(),calc->GetBin());
    LOG_DEBUG(QString("Command is : '%1'").arg(command));
    // -- ajout du process à la liste
    _pending_processes.append(cp);
    // -- lancement du processus
    cp->start(command);
    // -- write in process stdin
    switch (op) {
    case PluginProcess::SPLIT: // utile côté serveur
        cp->write(CS_OP_SPLIT);
        cp->write(calc->ToJson().toUtf8().data()); // ici calc est supposé être un ensemble de fragments
        cp->write(CS_EOF);
        break;
    case PluginProcess::JOIN: // utile côté serveur
        cp->write(CS_OP_JOIN);
        cp->write(calc->FragmentsResultsToJson().toUtf8().data()); // ici calc est supposé contenir un ensemble de fragment
        cp->write(CS_EOF);
        break;
    case PluginProcess::CALC: // utile côté client
        cp->write(CS_OP_CALC);
        cp->write(calc->ToJson().toUtf8().data()); // ici calc est supposé être un fragment
        cp->write(CS_EOF);
    default:
        LOG_CRITICAL("Processus started without arguments : unhandled operation is the cause !");
        break;
    }
    // -- on attend la fin du processus
    cp->waitForFinished();
}

void PluginManager::Slot_terminate()
{   LOG_DEBUG("Slot_terminate() called.");
    // -- kill all pending processes
    while(!_pending_processes.isEmpty())
    {   PluginProcess * cp = _pending_processes.takeFirst();
        cp->kill();
        cp->waitForFinished();
        delete cp;
    }
    // -- emit sig_terminated
    LOG_DEBUG("sig_terminated() emitted.");
    emit sig_terminated();
}

PluginManager::PluginManager() :
    _plugins_dir(),
    _pending_processes()
{
}
