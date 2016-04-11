#include "pluginmanager.h"
#include "src/const.h"
#include "src/utils/logger.h"
#include "src/calculation/specs.h"

#include <QCoreApplication>
#include <QFile>

#define ENTRY_LIST_FILTER   QDir::Files
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
    startProcess(calc, PluginProcess::SPLIT);
}

void PluginManager::Join(Calculation *calc)
{   // -- lancement du processus associé
    startProcess(calc, PluginProcess::JOIN);
}

bool PluginManager::WritePlugin(QString fname, const QByteArray &data)
{
    QFile f(fname.prepend('/').prepend(_plugins_dir.absolutePath()));
    if(!f.open(QIODevice::WriteOnly))
    {
        LOG_ERROR("Can't create plugin file.");
        return false;
    }
    f.write(data);
    f.close();
    return true;
}

void PluginManager::Slot_calc(Calculation *calc)
{   // -- lancement du processus associé
    startProcess(calc, PluginProcess::CALC);
}

void PluginManager::startProcess(Calculation * calc, PluginProcess::Operation op)
{
    // -- création d'un nouveau processus
    PluginProcess * cp = new PluginProcess(_plugins_dir.absolutePath(), calc, op);
    // -- ajout du process à la liste
    _processes.append(cp);
    // -- lancement du processus
    if(!cp->Start())
    {   // on spécifie qu'il y a eu une erreur au niveau de l'execution (elle n'a pas eu lieu)
        calc->Slot_crashed("Plugin type is script but no interpreter was found : process execution skipped !");
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
        cp->write(calc->FragmentsToJson().toUtf8().data()); // ici calc est supposé contenir un ensemble de fragment
        cp->write(CS_CRLF);
        cp->write(CS_EOF);
        cp->write(CS_CRLF);
        break;
    case PluginProcess::CALC: // utile côté client
        cp->write(CS_OP_CALC);
        cp->write(CS_CRLF);
        cp->write(calc->ToJson().toUtf8().data()); // ici calc est supposé être un fragment
        cp->write(CS_CRLF);
        cp->write(CS_EOF);
        LOG_DEBUG(QString("JSON param is : '%1'").arg(calc->ToJson().toUtf8().data()));
        cp->write(CS_CRLF);
        break;
    default:
        LOG_CRITICAL("Processus started without arguments : unhandled operation is the cause !");
        break;
    }
    cp->waitForFinished();
}

void PluginManager::Slot_stop()
{
    LOG_DEBUG("Slot_stop() called.");
    if (_processes.size() > 0 && _processes.first()->state() == QProcess::Running)
        _processes.first()->kill();
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
