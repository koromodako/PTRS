#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <QThread>
#include "src/const.h"
#include "src/calculation/calculation.h"
#include "src/calculation/calculationorder.h"

/**
 * @brief Cette classe gère les interactions avec l'utilisateur en console.
 *  Elle s'exécute dans un thread en parallèle de la boucle d'évènement principale de Qt.
 */
class ConsoleHandler : public QThread
{
    Q_OBJECT
public:
    ~ConsoleHandler(){}
    /**
     * @brief Récupère l'instance unique de cette classe
     * @return
     */
    static ConsoleHandler & GetInstance() { return _instance; }


protected:
    /**
     * @brief run
     */
    void run();

public slots:
    /**
     * @brief SLOT_RESPONSE
     * @param command
     * @param ok
     * @param calculations
     */
    void SLOT_RESPONSE(Command command, bool ok, QString message);

signals:
    /**
     * @brief SIG_EXEC
     * @param calculationOrder
     */
    void SIG_EXEC(CalculationOrder & calculationOrder);
    /**
     * @brief SIG_STATUS
     */
    void SIG_STATUS();
    /**
     * @brief SIG_RESULT
     * @param id
     */
    void SIG_RESULT(uint id);
    /**
     * @brief SIG_CANCEL
     * @param id
     */
    void SIG_CANCEL(uint id);
    /**
     * @brief SIG_SHUTDOWN
     */
    void SIG_SHUTDOWN();

private:
    ConsoleHandler();
    Q_DISABLE_COPY(ConsoleHandler)
    static ConsoleHandler _instance;

};

#endif // CONSOLEHANDLER_H
