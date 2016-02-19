#ifndef CONST_H
#define CONST_H

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

#endif // CONST_H
