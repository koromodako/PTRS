#include "calculationprocess.h"
#include "src/calculation/specs.h"
#include "src/utils/logger.h"

CalculationProcess::CalculationProcess(Calculation *calc, Operation op, QObject *parent) :
    QProcess(parent),
    _calculation(calc),
    _op(op),
    _out(""),
    _err("")
{
    // -- connexion du calcul aux évènements du processus
    connect(this, SIGNAL(error(QProcess::ProcessError)),      SLOT(SLOT_ERROR(QProcess::ProcessError)));
    connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(SLOT_FINISHED(int,QProcess::ExitStatus)));
}

void CalculationProcess::SLOT_ERROR(QProcess::ProcessError error)
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
    _calculation->Crashed(msg);
}

void CalculationProcess::SLOT_FINISHED(int exitCode, QProcess::ExitStatus exitStatus)
{   LOG_DEBUG(QString("SLOT_FINISHED(%1,%2) called.").arg(exitCode).arg(exitStatus));
    switch (exitStatus) {
    case QProcess::NormalExit:
        switch (_op) {
        case SPLIT:
            _calculation->Splitted(readAllStandardOutput());
            break;
        case JOIN:
            _calculation->Joined(readAllStandardOutput());
            break;
        case CALC:
            _calculation->Computed(readAllStandardOutput());
            break;
        }
        break;
    case QProcess::CrashExit:
        LOG_ERROR(QString("Process crashed (exit_code=%1).").arg(exitCode));
        // cancel calculation
        _calculation->Crashed(readAllStandardError());
        break;
    }
}
