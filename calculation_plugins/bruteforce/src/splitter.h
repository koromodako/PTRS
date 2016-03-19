#ifndef SPLITTER_H
#define SPLITTER_H

#include <QString>

class Splitter
{
public:
    Splitter();

    bool split(const QString & json);
    inline QString error() const { return _error; }
    inline QString result() const { return _result; }

private:
    QString _error;
    QString _result;

};

#endif // SPLITTER_H
