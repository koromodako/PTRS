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
                case LVL_NO_LVL:    level = "NO_LVL"; break;
                case LVL_DEBUG:     level = "DEBUG"; break;
                case LVL_INFO:      level = "INFO"; break;
                case LVL_ERROR:     level = "ERROR"; break;
                case LVL_CRITICAL:  level = "CRITICAL"; break;
                case LVL_FATAL:     level = "FATAL"; break;
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
}

Logger::Logger() : _config() {}
