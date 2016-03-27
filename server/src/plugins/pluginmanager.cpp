#include "pluginmanager.h"
#include "src/const.h"
#include "src/utils/logger.h"
#include "src/calculation/specs.h"

#include <QCoreApplication>
#include <QFile>

#define ENTRY_LIST_FILTER   QDir::Files | QDir::Executable
#define ENTRY_LIST_SORT     QDir::Name
#define ENTRY_LIST()        _plugins_dir.entryList(ENTRY_LIST_FILTER, ENTRY_LIST_SORT)

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
    startCalcProcess(calc, PluginProcess::SPLIT);
}

void PluginManager::Join(Calculation *calc)
{   // -- lancement du processus associé
    startCalcProcess(calc, PluginProcess::JOIN);
}

void PluginManager::Ui(Calculation *calc)
{   // -- lancement du processus associé
    startCalcProcess(calc, PluginProcess::UI);
}

void PluginManager::startCalcProcess(Calculation * calc, PluginProcess::CalculationOperation op)
{
    // -- création d'un nouveau processus
    PluginProcess * cp = new PluginProcess(_plugins_dir.absolutePath(), calc, op);
    // -- ajout du process à la liste
    _processes.append(cp);
    // -- lancement du processus
    if(!cp->Start())
    {   // on spécifie qu'il y a eu une erreur au niveau de l'execution (elle n'a pas eu lieu)
        calc->Crashed("Plugin type is script but no interpreter was found : process execution skipped !");
        // interruption de la routine
        return;
    }
    // -- on attend que le process se lance
    cp->waitForStarted();
    // -- write in process stdin
    switch (op) {
    case PluginProcess::SPLIT: // utile côté serveur
        cp->write(CS_OP_SPLIT);
        cp->write(CS_CRLF);
        cp->write(calc->ToJson().toUtf8().data()); // ici calc est supposé être un ensemble de fragments
        cp->write(CS_CRLF);
        cp->write(CS_EOF);
        cp->write(CS_CRLF);
        break;
    case PluginProcess::JOIN: // utile côté serveur
        cp->write(CS_OP_JOIN);
        cp->write(CS_CRLF);
        cp->write(calc->FragmentsResultsToJson().toUtf8().data()); // ici calc est supposé contenir un ensemble de fragment
        cp->write(CS_CRLF);
        cp->write(CS_EOF);
        cp->write(CS_CRLF);
        break;
    case PluginProcess::UI: // utile côté serveur
        cp->write(CS_OP_PARAM);
        cp->write(CS_CRLF);
        cp->write(CS_EOF);
        cp->write(CS_CRLF);
        break;
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
    while(!_processes.isEmpty())
    {   PluginProcess * cp = _processes.takeFirst();
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
    _processes()
{
}
