#include "../../server/src/calculation/specs.h"
#include "splitter.h"
#include "joiner.h"
#include "computer.h"

#include <iostream>
#include <QUrl>
#include <QString>
#include <QStringList>

void success(std::string response);
void fail(std::string msg);

int main(int argc, char *argv[])
{
    // récupération des arguments en sautant le nom du programme
    QStringList args;
    for(int i = 1; i < argc; ++i) { args.append(QString(argv[i])); }
    // traitement principal
    if(args.count() != 2)
    {   fail(QString("Expecting exactly 2 arguments : (%1|%2|%3) <json_url_encoded>").arg(CS_OP_SPLIT, CS_OP_CALC, CS_OP_JOIN).toStdString());
    }
    else
    {   QString action = args[0];
        QString json = QUrl::fromPercentEncoding(args[1].toUtf8());
        if(action == CS_OP_JOIN)
        {   Joiner joiner;
            if(joiner.join(json))
            {   success(QString(QUrl::toPercentEncoding(joiner.result())).toStdString());
            }
            else
            {   fail(joiner.error().toStdString());
            }
        }
        else if(action == CS_OP_SPLIT)
        {   Splitter splitter;
            if(splitter.split(json))
            {   success(QString(QUrl::toPercentEncoding(splitter.result())).toStdString());
            }
            else
            {   fail(splitter.error().toStdString());
            }
        }
        else if(action == CS_OP_CALC)
        {   Computer computer;
            if(computer.compute(json))
            {   success(QString(QUrl::toPercentEncoding(computer.result())).toStdString());
            }
            else
            {   fail(computer.error().toStdString());
            }
        }
        else
        {   fail("Unknown operation.");
        }
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
