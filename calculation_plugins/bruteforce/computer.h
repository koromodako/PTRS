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

    bool bruteForceRecursif(std::set<QChar> charset, QString prefixe, uint longueur, uint longueur_max, QString target);
    bool bruteForce(std::set<QChar> charset, uint length, QString target);
    bool decideHashAlgorithm(QString requested_algorithm);
    bool possibleCharacters(std::set<QChar> *possible_characters, QString charset);
};

#endif // COMPUTER_H
