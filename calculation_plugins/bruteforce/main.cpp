#include "../../server/src/calculation/specs.h"
#include "splitter.h"
#include "joiner.h"
#include "computer.h"

#include <iostream>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <stdio.h>

void success(std::string response);
void fail(std::string msg);

int main(int argc, char *argv[])
{
    // récupération des paramètres de calcul
    QTextStream qtin(stdin);
    QString action;
    QString json;
    qtin >> action;
    for(QString line; line != CS_EOF; line = qtin.readLine()) json += line;

    // traitement principal    
    if(action == CS_OP_JOIN)
    {   Joiner joiner;
        if(joiner.join(json))
        {   success(QString(joiner.result()).toStdString());
        }
        else
        {   fail(joiner.error().toStdString());
        }
    }
    else if(action == CS_OP_SPLIT)
    {   Splitter splitter;
        if(splitter.split(json))
        {   success(QString(splitter.result()).toStdString());
        }
        else
        {   fail(splitter.error().toStdString());
        }
    }
    else if(action == CS_OP_CALC)
    {   Computer computer;
        if(computer.compute(json))
        {   success(QString(computer.result()).toStdString());
        }
        else
        {   fail(computer.error().toStdString());
        }
    }
    else
    {   fail("Unknown operation.");
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
