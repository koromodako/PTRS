#include "../common/calculation_specs.h"
#include "splitter.h"
#include "joiner.h"

#include <QString>
#include <iostream>
#include <QStringList>

void success(std::string response);
void fail(std::string msg);

int main(int argc, char *argv[])
{
    // récupération des arguments en sautant le nom du programme
    QStringList args;
    for(int i = 1; i < argc; ++i) { args.append(QString(argv[i])); }
    // traitement principal
    if(args.length() > 0)
    {   QString action = args.takeFirst();
        if(action == CS_OP_JOIN)
        {   Joiner joiner;
            if(joiner.join(args))
            {   success(joiner.result().toStdString());
            }
            else
            {   fail(joiner.error().toStdString());
            }
        }
        else if(action == CS_OP_SPLIT)
        {   Splitter splitter;
            if(splitter.split(args))
            {   success(splitter.result().toStdString());
            }
            else
            {   fail(splitter.error().toStdString());
            }
        }
        else
        {   fail("Unknown operation.");
        }
    }
    else
    {   fail("Missing at least one argument");
    }
    // we should never reach this point
    exit(EXIT_FAILURE);
}

void success(std::string response)
{   std::cout << response << std::endl;
    exit(EXIT_SUCCESS);
}

void fail(std::string msg)
{   std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
