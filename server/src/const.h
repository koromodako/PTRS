#ifndef CONST_H
#define CONST_H

#include <QString>

/**
 * @brief Cette énumération décrit les différentes commandes pouvant être traitées par le console handler.
 * @see ConsoleHandler
 */
enum Command {
    CMD_EXEC,
    CMD_STATUS,
    CMD_RESULT,
    CMD_CANCEL,
    CMD_SHUTDOWN
};

/**
 * @brief Cette enumeration décrit les différents niveaux de log possible
 */
enum Level {
    LVL_NO_LVL      = 0x00,
    LVL_DEBUG       = 0x01,
    LVL_INFO        = 0x02,
    LVL_ERROR       = 0x04,
    LVL_CRITICAL    = 0x08,
    LVL_FATAL       = 0x10
};


#endif // CONST_H
