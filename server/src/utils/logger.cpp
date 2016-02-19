#include "logger.h"

#include <iostream>
#include <QDateTime>

Logger Logger::_instance;

void Logger::Log(Logger::Level lvl, QString message, QString line, QString file, bool endl) const
{
    QString log;
    if(lvl == LVL_NO_LVL)
    {   log = message;
    }
    else
    {   // retrieve format
        log = _config.format;
        // substitute
        log.replace("%c", levelToString(lvl))
           .replace("%t", QDateTime::currentDateTime().toString(Qt::ISODate))
           .replace("%m", message)
           .replace("%f", file)
           .replace("%l", line);
    }
    // print log
    std::cerr << log.toStdString();
    // add endl if required
    if(endl) { std::cerr << std::endl; }
}

Logger::Logger() : _config() {}

QString Logger::levelToString(Logger::Level lvl) const
{
    switch (lvl) {
        case LVL_NO_LVL:    return "NO_LVL";
        case LVL_DEBUG:     return "DEBUG";
        case LVL_INFO:      return "INFO";
        case LVL_ERROR:     return "ERROR";
        case LVL_CRITICAL:  return "CRITICAL";
        case LVL_FATAL:     return "FATAL";
    }
    return "unhandled_log_lvl";
}
