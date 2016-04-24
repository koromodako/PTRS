#include "consolehandler.h"

#include "src/utils/logger.h"

#include <stdio.h>
#include <QStringList>
#include <QMutexLocker>

// -- define command keywords
#define C_STATE       "STATE"
#define C_HELP        "HELP"
#define C_SHUTDOWN    "SHUTDOWN"
#define C_CONNECT     "CONNECT"

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
    LOG_DEBUG("Slot_response called");
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
    respond("Welcome to client CLI !");
}

bool ConsoleHandler::prompt(QString * input)
{
     _out << "client <: " << flush;
    *input = _in.readLine();
    if (input->isNull())
        return false;
    return true;
    //J'ai remplacé le code ci dessous car j'ai pas Qt5.5 - rémy
    //return _in.readLineInto(input);
}

void ConsoleHandler::respond(QString response)
{
    print(response.prepend("client :> "));
}

void ConsoleHandler::error(QString errorStr, QString cmd)
{
    QString details("");
    if(!cmd.isEmpty())
    {   details.append("about ").append(cmd).append(" command");
    }
    QString msg = QString(" Type %1 %2to get some hints and tips%3. ").arg(C_HELP,cmd.append(' '),details.prepend(' '));
    respond(errorStr.append(msg));
}

void ConsoleHandler::print(const QString & string, bool eol)
{
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
                "\t\t+ " C_HELP " [<command>] : print client's help or a specific command help.\n"
                "\n"
                "\t\t+ " C_STATE " : print client state.\n"
                "\n"
                "\t\t+ " C_CONNECT " : connect to the server.\n"
                "\n"
                "\t\t+ " C_SHUTDOWN " : shutdown client.\n"
                "\n");
    }
    else
    {   if(cmd == C_HELP)
        {   respond(C_HELP" [<command>] : print client's help or a specific command help.");
        }
        else if(cmd == C_STATE)
        {   respond(C_STATE" : print client state.");
        }
        else if(cmd == C_CONNECT)
        {   respond(C_CONNECT" : connect to the server.");
        }
        else if(cmd == C_SHUTDOWN)
        {   respond(C_SHUTDOWN" : shutdown client.");
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
        else if(args[0] == C_CONNECT)
        {
            EMIT_AND_WAIT(sig_connect());
        }
        HANDLE_ERR("Unknown command.")
    }
    HANDLE_ERR("Missing command keyword !")
    // return true if console handler is waiting for a response
    return wait;
}

ConsoleHandler::ConsoleHandler() :
    _out(stdout), _in(stdin)
{
}
