#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QMutex>
#include "src/const.h"

#define DEBUG

// -- definition de quelques format de log
#define LOG_FORMAT_LIGHT       "[%c] - %m"
#define LOG_FORMAT_TIMED       "(%t)[%c] - %m"
#define LOG_FORMAT_DETAILED    "(%t)[%c] - %m - {%f:%l}"

/**
 * @brief Cette structure permet de configurer le logger
 *
 * Elle permet de configurer deux aspects du logger :
 *      - quels niveaux de messages doivent être loggés :
 *          ex : disable_flags = LVL_DEBUG | LVL_INFO => les logs de niveaux DEBUG et INFO ne seront pas affichés
 *
 *      - quel format doit être utilisé pour l'affichage :
 *          %t : timestamp format ISO
 *          %c : criticité du message (niveau)
 *          %m : contenu du message
 *          %f : fichier contenant la ligne de logging
 *          %l : ligne dans le fichier
 */
struct LoggerConfiguration {
    int disable_flags;
    QString format;
    LoggerConfiguration() :
        disable_flags(0),           // default -> all log levels
        format(LOG_FORMAT_LIGHT)    // default -> light log format
    {}
};

/**
 * @brief Cette classe permet de générer des logs dans la sortie d'erreur standard (cerr)
 */
class Logger
{
public:    
    ~Logger(){}
    static Logger & GetInstance() { return _instance;  }

    void Configure(LoggerConfiguration & config) { _config = config; }

    void Log(Level lvl, QString message, QString line = QString(), QString file = QString(), bool endl = true) const;

    /**
     * @brief Defini le mutex à utiliser pour les entrées/sorties
     */
    void SetConsoleMutex(QMutex *mutex);

private: // singleton
    Logger();
    Q_DISABLE_COPY(Logger)
    static Logger _instance;

private:
    LoggerConfiguration _config;
};

// -- macros pour logger facilement
// --- configuration
#define LOGGER_CONFIGURE(flags, format_string) \
    LoggerConfiguration config; \
    config.disable_flags = flags; \
    config.format = format_string; \
    Logger::GetInstance().Configure(config)
// --- logging
#define __QFILE__ QString(__FILE__)
#define __QLINE__ QString::number(__LINE__)
#define LOG_INFO(msg) Logger::GetInstance().Log(LVL_INFO, msg, __QLINE__, __QFILE__)
#define LOG_WARN(msg) Logger::GetInstance().Log(LVL_WARN, msg, __QLINE__, __QFILE__)
#define LOG_ERROR(msg) Logger::GetInstance().Log(LVL_ERROR, msg, __QLINE__, __QFILE__)
#define LOG_CRITICAL(msg) Logger::GetInstance().Log(LVL_CRITICAL, msg, __QLINE__, __QFILE__)
#define LOG_FATAL(msg) Logger::GetInstance().Log(LVL_FATAL, msg, __QLINE__, __QFILE__)

#ifdef DEBUG
#   define LOG_DEBUG(msg) Logger::GetInstance().Log(LVL_DEBUG, msg, __QLINE__, __QFILE__)
#else
#   define LOG_DEBUG(msg)
#endif


#endif // LOGGER_H
