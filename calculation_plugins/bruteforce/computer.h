#ifndef COMPUTER_H
#define COMPUTER_H

#include <QString>

class Computer
{
public:
    Computer();

    bool compute(const QString & json);
    inline QString error() const { return _error; }
    inline QString result() const { return _result; }

private:
    QString _error;
    QString _result;
};

#endif // COMPUTER_H
