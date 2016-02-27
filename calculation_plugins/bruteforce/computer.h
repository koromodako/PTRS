#ifndef COMPUTER_H
#define COMPUTER_H

#include <QString>
#include <set>
#include <QCryptographicHash>

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

    QString _match_string;
    QCryptographicHash::Algorithm _hash_algorithm;

    bool bruteForceRecursif(std::set<char> charset, QString prefixe, uint longueur, uint longueur_max, QString target);
    bool bruteForce(std::set<char> charset, uint length, QString target);
    bool decideHashAlgorithm(QString requested_algorithm);
};

#endif // COMPUTER_H
