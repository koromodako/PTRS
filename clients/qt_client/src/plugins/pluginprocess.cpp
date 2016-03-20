#include "pluginprocess.h"
#include "src/calculation/specs.h"
#include "src/utils/logger.h"

#include <QUrl>

PluginProcess::PluginProcess(QString absExecDir, Calculation *calc, Operation op, QObject *parent) :
    QProcess(parent),
    _absExecDir(absExecDir),
    _calculation(calc),
    _op(op),
    _out(""),
    _err("")
{
    // -- connexion du calcul aux évènements du processus
    connect(this, SIGNAL(error(QProcess::ProcessError)),      SLOT(SLOT_ERROR(QProcess::ProcessError)));
    connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(SLOT_FINISHED(int,QProcess::ExitStatus)));
}

bool PluginProcess::Start()
{
    // construction de la commande en fonction du type de binaire
    QString command(_absExecDir);
    // on ajoute le nom du binaire à la fin
    command.append('/').append(_calculation->GetBin());
    // en fonction du type on effectue des opérations supplémentaires
    bool ok = true;
    switch (detectType()) {
    case BINARY: break;
    case JAR:
        command.prepend("java -jar ");
        break;
    case SCRIPT:
        if(! selectInterpreter().isNull() )
        {   command.prepend(selectInterpreter()).prepend(" ");
        }
        else
        {   ok = false;
        }
        break;
    }
    // -- démarrage du plugin
    if(ok)
    {   // on exécute la commande trimmée pour éviter les espaces traitres
        start(command.trimmed());
    }
    // -- retour du statut
    return ok;
}

void PluginProcess::SLOT_ERROR(QProcess::ProcessError error)
{   LOG_DEBUG(QString("SLOT_ERROR(%1) called.").arg(error));
    QString msg("");
    switch (error) {
    case QProcess::FailedToStart:
        msg.append("process failed to start.");
        break;
    case QProcess::Crashed:
        msg.append("process crashed.");
        break;
    case QProcess::Timedout:
        msg.append("process timed out.");
        break;
    case QProcess::WriteError:
        msg.append("process write error.");
        break;
    case QProcess::ReadError:
        msg.append("process read error.");
        break;
    case QProcess::UnknownError:
        msg.append("unknown error.");
        break;
    }
    _calculation->Slot_crashed(msg);
}

void PluginProcess::SLOT_FINISHED(int exitCode, QProcess::ExitStatus exitStatus)
{   LOG_DEBUG(QString("SLOT_FINISHED(%1,%2) called.").arg(exitCode).arg(exitStatus));
    switch (exitStatus) {
    case QProcess::NormalExit:
        if(exitCode == 0)
        {
            _calculation->Slot_computed(QUrl::fromPercentEncoding(readAllStandardOutput()).toUtf8());
        }
        else
        {   LOG_ERROR(QString("Process crashed (exit_code=%1).").arg(exitCode));
            // crash calculation
            _calculation->Slot_crashed(readAllStandardError());
        }
        break;
    case QProcess::CrashExit:
        LOG_ERROR(QString("Process crashed (exit_code=%1).").arg(exitCode));
        // crash calculation
        _calculation->Slot_crashed(readAllStandardError());
        break;
    }
}

#define JAR_EXT "jar"
#define SCRIPT_EXT() QStringList({"py","sh"})
#define SCRIPT_INTERPRETER() QStringList({"python", "bash"})

PluginProcess::Type PluginProcess::detectType()
{
    Type type = BINARY;
    QStringList parts = _calculation->GetBin().split('.', QString::SkipEmptyParts);
    if(!parts.isEmpty())
    {   if(parts.last() == JAR_EXT)
        {   type = JAR;
        }
        else if(SCRIPT_EXT().contains(parts.last()))
        {   type = SCRIPT;
        }
    }
    return type;
}

QString PluginProcess::selectInterpreter()
{
    QStringList parts = _calculation->GetBin().split('.', QString::SkipEmptyParts);
    int index(-1);
    if(!parts.isEmpty())
    {   index = SCRIPT_EXT().indexOf(parts.last());
    }
    return (index >= 0 ? SCRIPT_INTERPRETER()[index] : QString());
}
