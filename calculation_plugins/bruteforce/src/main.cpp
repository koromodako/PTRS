#include "../../server/src/calculation/specs.h"
#include "bruteforce_specs.h"
#include "splitter.h"
#include "joiner.h"
#include "computer.h"

#include <iostream>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <stdio.h>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

void success(std::string response);
void fail(std::string msg);
static QJsonValue getParam(QString name, QString type, QString regex, QString tooltip);
static QJsonValue getParam(QString name, QString type, int minimum);

int main(int argc, char *argv[])
{
    // récupération des paramètres de calcul
    QTextStream qtin(stdin);
    QString action;
    QString json;
    qtin >> action;
    for(QString line; !qtin.atEnd() && line != CS_EOF; line = qtin.readLine()) json += line;

    // traitement principal    
    if(QString::compare(action, CS_OP_JOIN, Qt::CaseInsensitive) == 0)
    {   Joiner joiner;
        if(joiner.join(json))
        {   success(QString(joiner.result()).toStdString());
        }
        else
        {   fail(joiner.error().toStdString());
        }
    }
    else if(QString::compare(action, CS_OP_SPLIT, Qt::CaseInsensitive) == 0)
    {   Splitter splitter;
        if(splitter.split(json))
        {   success(QString(splitter.result()).toStdString());
        }
        else
        {   fail(splitter.error().toStdString());
        }
    }
    else if(QString::compare(action, CS_OP_CALC, Qt::CaseInsensitive) == 0)
    {   Computer computer;
        if(computer.compute(json))
        {   success(QString(computer.result()).toStdString());
        }
        else
        {   fail(computer.error().toStdString());
        }
    }
    else if(QString::compare(action, CS_OP_PARAM, Qt::CaseInsensitive) == 0)
    {
        QJsonDocument retDocument;

        QJsonArray listParams;

        listParams += getParam(PARAM_CHARSET, CS_TYPE_STRING, REGEX_NOT_EMPTY, TOOLTIP_NOT_EMPTY);
        listParams += getParam(PARAM_MIN_LEN, CS_TYPE_INT, 1);
        listParams += getParam(PARAM_MAX_LEN, CS_TYPE_INT, 1);
        listParams += getParam(PARAM_HASH_F, CS_TYPE_STRING, REGEX_HASH_FUNCTIONS, TOOLTIP_HASH_FUNCTIONS);
        listParams += getParam(PARAM_TARGET, CS_TYPE_STRING, REGEX_NOT_EMPTY, TOOLTIP_NOT_EMPTY);

        retDocument.setArray(listParams);

        success(QString(retDocument.toJson(QJsonDocument::Compact)).toStdString());
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

static QJsonValue getParam(QString name, QString type, QString regex, QString tooltip)
{
    QJsonObject obj;
    obj.insert(CS_PLUGINPARAMS_NAME, name);
    obj.insert(CS_PLUGINPARAMS_TYPE, type);
    obj.insert(CS_PLUGINPARAMS_REGEX, regex);
    obj.insert(CS_PLUGINPARAMS_TOOLTIP, tooltip);

    return obj;
}

static QJsonValue getParam(QString name, QString type, int minimum)
{
    QJsonObject obj;
    obj.insert(CS_PLUGINPARAMS_NAME, name);
    obj.insert(CS_PLUGINPARAMS_TYPE, type);
    obj.insert(CS_PLUGINPARAMS_MIN, 1);

    return obj;
}
