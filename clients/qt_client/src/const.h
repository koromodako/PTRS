#ifndef CONST_H
#define CONST_H

#define PLUGINS_DIR "plugins"

#include <QString>
#include <QObject>

/**
 * @brief Cette énumération décrit les différentes commandes pouvant être traitées par le console handler.
 * @see ConsoleHandler
 */
enum Command {
    CMD_CONNECT,
    CMD_SHUTDOWN,
    CMD_STATE
};
Q_DECLARE_METATYPE(Command)

/**
 * @brief Cette enumeration décrit les différents niveaux de log possible
 */
enum Level {
    LVL_NO_LVL      = 0x00,
    LVL_DEBUG       = 0x01,
    LVL_INFO        = 0x02,
    LVL_WARN        = 0x04,
    LVL_ERROR       = 0x08,
    LVL_CRITICAL    = 0x10,
    LVL_FATAL       = 0x20
};

/**
 * @brief Cette énumération décrit les différentes commandes échangables entre le client et le serveur
 */
enum ReqType {
    HELLO               = 0x01,
    HELLO_FROM_SERVER   = 0x02,
    READY               = 0x03,
    WORKING             = 0x04,
    UNABLE              = 0x05,
    DONE                = 0x06,
    ABORT               = 0x07,
    OK                  = 0x08,
    KO                  = 0x09,
    DO                  = 0x0A,
    STOP                = 0x0B,
    BIN                 = 0x0C
};


#endif // CONST_H
