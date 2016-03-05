#include "consolehandler.h"

#include "src/utils/logger.h"

#include <stdio.h>
#include <QStringList>
#include <QMutexLocker>

// -- define command keywords
#define C_STATE       "STATE"
#define C_HELP        "HELP"
#define C_SHUTDOWN    "SHUTDOWN"
#define C_EXEC        "EXEC"
#define C_STATUS      "STATUS"
#define C_RESULT      "RESULT"
#define C_CANCEL      "CANCEL"

// -- define some convenient macros shrink code
#define HANDLE_ERR(msg) else { error(msg); }
#define HANDLE_SPE_ERR(msg, cmd) else { error(msg, cmd); }
#define EMIT_AND_WAIT(signal) emit signal; wait = true

ConsoleHandler ConsoleHandler::_instance;

void ConsoleHandler::Slot_init()
{
    LOG_INFO("Démarrage du console handler...");

    // say hello
    welcome();
    // loop while user input
    QString input;
    while(prompt(&input) && !interpret(input));
}

void ConsoleHandler::Slot_response(Command command, bool ok, QString message)
{
    //LOG_DEBUG("Slot_response called");
    if(command == CMD_SHUTDOWN) {
        // say goodbye
        goodbye();
        // notify termination
        emit sig_terminated();
    } else {
        respond(message);
        QString input;
        while (prompt(&input) && !interpret(input));
    }
}

void ConsoleHandler::welcome()
{
    respond("Welcome to server CLI !");
}

bool ConsoleHandler::prompt(QString * input)
{
    QMutexLocker locker(_consoleMutex);
     _out << "server <: " << flush;
    *input = _in.readLine();
    if (input->isNull())
        return false;
    return true;
    //J'ai remplacé le code ci dessous car j'ai pas Qt5.5 - rémy
    //return _in.readLineInto(input);
}

void ConsoleHandler::respond(QString response)
{
    print(response.prepend("server :> "));
}

void ConsoleHandler::error(QString error_str, QString cmd)
{
    QString details("");
    if(!cmd.isEmpty())
    {   details.append("about ").append(cmd).append(" command");
    }
    QString msg = QString(" Type %1 %2to get some hints and tips%3. ").arg(C_HELP,cmd.append(' '),details.prepend(' '));
    respond(error_str.append(msg));
}

void ConsoleHandler::print(const QString & string, bool eol)
{
    QMutexLocker locker(_consoleMutex);
    _out << string;
    if(eol) { _out << endl; }
    _out << flush;
}

void ConsoleHandler::help(const QString &cmd)
{
    if(cmd.isEmpty())
    {   respond("\nGeneral help for server CLI interactions.\n"
                "\n"
                "\tAvailable commands:\n"
                "\n"
                "\t\t+ " C_HELP " [<command>] : print server's help or a specific command help.\n"
                "\n"
                "\t\t+ " C_STATE " : print server state.\n"
                "\n"
                "\t\t+ " C_SHUTDOWN " : shutdown server.\n"
                "\n"
                "\t\t+ " C_STATUS " : print calculations statuses.\n"
                "\n"
                "\t\t+ " C_EXEC " <calculation_order_block> : print calculations statuses.\n"
                "\n"
                "\t\t+ " C_CANCEL " <id> : cancel a calculation identified using <id>.\n"
                "\n"
                "\t\t+ " C_RESULT " <id> [<filename>] : print or export calculation result using <id>.\n");
    }
    else
    {   if(cmd == C_HELP)
        {   respond(C_HELP" [<command>] : print server's help or a specific command help.");
        }
        else if(cmd == C_STATE)
        {   respond(C_STATE" : print server state.");
        }
        else if(cmd == C_SHUTDOWN)
        {   respond(C_SHUTDOWN" : shutdown server.");
        }
        else if(cmd == C_STATUS)
        {   respond(C_STATUS" : print calculations statuses.");
        }
        else if(cmd == C_EXEC)
        {   respond(C_EXEC" <calculation_order_block> : schedule new calculation.");
        }
        else if(cmd == C_CANCEL)
        {   respond(C_CANCEL" <id> : cancel a calculation identified using <id>.");
        }
        else if(cmd == C_RESULT)
        {   respond(C_RESULT" <id> [<filename>] : print or export calculation result using <id>.\n");
        }
        HANDLE_ERR(QString("Unknown command %1, no help will be displayed.").arg(cmd));
    }
}

void ConsoleHandler::goodbye()
{
    respond("Goodbye !");
}

bool ConsoleHandler::interpret(QString &input)
{
    bool wait = false;
    QStringList args = input.split(' ', QString::SkipEmptyParts);
    if(!args.empty())
    {   if(args[0] == C_STATE)
        {   EMIT_AND_WAIT(sig_state());
        }
        else if(args[0] == C_SHUTDOWN)
        {   EMIT_AND_WAIT(sig_shutdown());
        }
        else if(args[0] == C_HELP)
        {   if(args.size() > 1)
            {   help(args[1]); //specific command help will be printed
            }
            else
            {   help(); //general help will be printed
            }
        }
        else if(args[0] == C_EXEC) // exec command received
        {   if(args.size() > 1)
            {   EMIT_AND_WAIT(sig_exec(args[1].toUtf8()));
            }
            HANDLE_SPE_ERR("Missing argument.", C_EXEC)
        }
        else if(args[0] == C_STATUS)
        {  EMIT_AND_WAIT(sig_status());
        }
        else if(args[0] == C_RESULT)
        {   if(args.size() > 1)
            {   QUuid id(args[1].toUtf8());
                if(!id.isNull())
                {   if(args.size() > 2)
                    {   EMIT_AND_WAIT(sig_result(id, args[2]));
                    }
                    else
                    {   EMIT_AND_WAIT(sig_result(id));
                    }
                }
                HANDLE_SPE_ERR("Invalid id.", C_RESULT)
            }
            HANDLE_SPE_ERR("Missing argument.", C_RESULT)
        }
        else if(args[0] == C_CANCEL)
        {   if(args.size() > 1)
            {   QUuid id(args[1].toUtf8());
                if(!id.isNull())
                {   EMIT_AND_WAIT(sig_cancel(id));
                }
                HANDLE_SPE_ERR("Invalid id.", C_CANCEL)
            }
            HANDLE_SPE_ERR("Missing argument.", C_CANCEL)
        }
        HANDLE_ERR("Unknown command.")
    }
    HANDLE_ERR("Missing command keyword !")
    // return true if console handler is waiting for a response
    return wait;
}

void ConsoleHandler::SetConsoleMutex(QMutex *mutex)
{
    _consoleMutex = mutex;
}


ConsoleHandler::ConsoleHandler() :
    _out(stdout), _in(stdin),
    _consoleMutex(NULL)
{
}
