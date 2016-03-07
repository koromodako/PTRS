#include "computer.h"
#include "../../server/src/calculation/specs.h"
#include "bruteforce_specs.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <iostream>

bool Computer::compute(const QString &json)
{
    // drapeau ok initialisé baissé
    bool ok = false;
    // récupération du calcul à fragmenter à partir du json
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &error);
    QVariantMap params;
    if(!error.error && doc.isObject())
    {   if(doc.object().contains(CS_JSON_KEY_CALC_PARAMS) && doc.object().value(CS_JSON_KEY_CALC_PARAMS).isObject())
        {   params = doc.object().value(CS_JSON_KEY_CALC_PARAMS).toObject().toVariantMap();

            // récupération des paramètres du calcul
            QString charset = params.value(PARAM_CHARSET).toString();
            uint minLen = params.value(PARAM_MIN_LEN).toInt();
            uint maxLen = params.value(PARAM_MAX_LEN).toInt();
            QString hashFunction = params.value(PARAM_HASH_F).toString();
            QString target = params.value(PARAM_TARGET).toString();

            // récupération de l'algo de hashage
            if(!decideHashAlgorithm(hashFunction)) return false;

            // génération du jeu de charactères
            std::set<QChar> characters;
            if(!possibleCharacters(&characters, charset)) return false;

            // calcul brute force
            bool matchFound = false;
            for(uint i=minLen; i<=maxLen && !matchFound; i++)
            {   matchFound = bruteForce(characters, i, target);
            }

            // construction de la réponse
            QVariantMap mapResult;
            mapResult.insert(PARAM_HAS_MATCH, matchFound);
            mapResult.insert(PARAM_MATCH_STR, _match_string);
            QVariantMap mapResponse;
            mapResponse.insert(CS_JSON_KEY_FRAG_ID, (doc.object())[CS_JSON_KEY_FRAG_ID].toString());
            mapResponse.insert(CS_JSON_KEY_CALC_RESULT, mapResult);
            QJsonDocument response(QJsonObject::fromVariantMap(mapResponse));
            _result = response.toJson();

            // tout s'est bien passé, on lève le flag
            ok = true;

        }
        else
        {   _error = QString("Missing '%1' field in calculation !")
                    .arg(CS_JSON_KEY_CALC_PARAMS);
        }
    }
    else
    {   _error = "Missing json object !";
    }

    return ok;
}

Computer::Computer() :
    _error(""),
    _result(""),
    _match_string("")
{
}


bool Computer::bruteForce(std::set<QChar> charset, uint length, QString target)
{
    QString root = "";
    for(uint i=0; i<length; i++)
    {   root += *(charset.begin());
    }
    return bruteForceRecursif(charset, root, 1, length, target);
}


bool Computer::bruteForceRecursif(std::set<QChar> charset, QString prefixe, uint longueur, uint longueur_max, QString target)
{
    if(longueur == longueur_max)
    {
        for(std::set<QChar>::iterator charIter = charset.begin(); charIter != charset.end(); ++charIter)
        {
            // on génére et on teste une nouvelle proposition
            prefixe.replace(longueur_max - 1, 1, *charIter);
            QString hashed = QCryptographicHash::hash(prefixe.toUtf8(), _hash_algorithm).toHex();

            // si la cible a été trouvée, on met à jour le résultat et on arrête l'exploration récursive
            if(QString::compare(target, hashed, Qt::CaseInsensitive) == 0)
            {   _match_string = prefixe;
                return true;
            }
        }
        return false;
    }
    else
    {
        for(std::set<QChar>::iterator charIter = charset.begin(); charIter != charset.end(); ++charIter)
        {   prefixe.replace(longueur - 1, 1, *charIter);
            if(bruteForceRecursif(charset, prefixe, longueur + 1, longueur_max, target)) return true;
        }
        return false;
    }
}


// A modifier au besoin, en fonction du format retenu pour le charset
bool Computer::possibleCharacters(std::set<QChar> *possible_characters, QString charset)
{
    if(charset.size() == 0)
    { _error = QString("Incorrect parameter : '%1' !").arg(PARAM_CHARSET);
        return false;
    }

    for(int i = 0; i < charset.length(); i++)
    {   possible_characters->insert(charset.at(i));
    }

    return true;
}


bool Computer::decideHashAlgorithm(QString requested_algorithm)
{
    if (QString::compare(requested_algorithm, "Md4", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Md4;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Md5", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Md5;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha1", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha1;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha224", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha224;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha256", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha256;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha384", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha384;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha512", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha512;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha3_224", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha3_224;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha3_256", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha3_256;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha3_384", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha3_384;
        return true;
    }
    else if (QString::compare(requested_algorithm, "Sha3_512", Qt::CaseInsensitive) == 0)
    {   _hash_algorithm = QCryptographicHash::Sha3_512;
        return true;
    }

    _error = QString("Incorrect parameter : '%1' !").arg(PARAM_HASH_F);
    return false;
}


