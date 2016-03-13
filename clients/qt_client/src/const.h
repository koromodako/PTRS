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
    CMD_EXEC,
    CMD_STATUS,
    CMD_RESULT,
    CMD_CANCEL,
    CMD_SHUTDOWN,
    CMD_STATE
};
Q_DECLARE_METATYPE(Command);

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
    HELLO,
    HELLO_FROM_SERVER,
    READY,
    WORKING,
    UNABLE,
    DONE,
    ABORT,
    OK,
    KO,
    DO,
    STOP
};


#endif // CONST_H
