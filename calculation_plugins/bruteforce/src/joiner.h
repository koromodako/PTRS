#ifndef JOINER_H
#define JOINER_H

#include <QString>

class Joiner
{
public:
    Joiner();

    bool join(const QString &json);
    inline QString error() const { return _error; }
    inline QString result() const { return _result; }

private:
    QString _error;
    QString _result;

};

#endif // JOINER_H
