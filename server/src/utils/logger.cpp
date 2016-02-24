#include "logger.h"

#include <iostream>
#include <QDateTime>
#include <QStringList>

Logger Logger::_instance;

void Logger::Log(Level lvl, QString message, QString line, QString file, bool endl) const
{
    QString log;
    if(lvl == LVL_NO_LVL)
    {   log = message;
    }
    else
    {   // retrieve format
        log = _config.format;
        QString level("unhandled_log_lvl");
        switch (lvl) {
            case LVL_NO_LVL:    level = "NLVL"; break;
            case LVL_DEBUG:     level = "DBUG"; break;
            case LVL_INFO:      level = "INFO"; break;
            case LVL_WARN:      level = "WARN"; break;
            case LVL_ERROR:     level = "ERRO"; break;
            case LVL_CRITICAL:  level = "CRIT"; break;
            case LVL_FATAL:     level = "FATL"; break;
            }
        // substitute
        log.replace("%c", level)
           .replace("%t", QDateTime::currentDateTime().toString(Qt::ISODate))
           .replace("%m", message)
           .replace("%f", file.split('/').last())
           .replace("%l", line);
    }
    // print log
    std::cerr << log.toStdString();
    // add endl if required
    if(endl) { std::cerr << std::endl; }
    // flush
    std::cerr << std::flush;
}

Logger::Logger() : _config() {}
