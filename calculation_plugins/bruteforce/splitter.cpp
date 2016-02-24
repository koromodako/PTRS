#include "splitter.h"
#include "../../server/src/calculation/specs.h"
#include "bruteforce_specs.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <iostream>

#define AVG_TEST_PER_SEC            500     // pwd.s-1
#define OPTIMAL_COMPUTATION_TIME    60      // s

bool Splitter::split(const QString &json)
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
        }
        else
        {   _error = QString("Missing '%1' field in calculation !")
                    .arg(CS_JSON_KEY_CALC_PARAMS);
        }
    }
    else
    {   _error = "Missing json object !";
    }
    // vérification des paramètres
    if(params.contains(PARAM_MIN_LEN) &&
       params.contains(PARAM_MAX_LEN))
    {   /*
         *  La fragmentation stupide pour le bruteforce consiste a distribuer
         *  en donnant une taille de mot de passe pour chaque fragment
         */
        // -- récupération des valeurs de min et max len dans les champs des paramètres
        uint min_len = params.value(PARAM_MIN_LEN).toInt();
        uint max_len = params.value(PARAM_MAX_LEN).toInt();
        QStringList fragments;
        // -- pour chaque longueur entre min et max len, création d'un fragment
        for (uint l = min_len; l <= max_len; ++l) {
            // --- récupération des paramètres du calcul de base
            QVariantMap frag_params = params;
            // --- modification des champs min et max len dans les paramètre
            frag_params.insert(PARAM_MIN_LEN, l);
            frag_params.insert(PARAM_MAX_LEN, l);
            // --- récupération de l'objet calcul de base et mise à jour du champ paramètres
            QJsonObject frag = doc.object();
            // --- modification du champ paramètre
            frag.insert(CS_JSON_KEY_CALC_PARAMS, QJsonObject::fromVariantMap(frag_params));
            // --- insertion du fragment dans la liste
            fragments << QJsonDocument(frag).toJson(QJsonDocument::Compact);
        }
        // -- insertion des fragment dans l'attribut résultat de l'objet splitter
        _result = fragments.join(CS_FRAGMENT_SEP);
        // on lève le drapeau
        ok = true;
    }
    else
    {   _error = QString("Missing fields '%1' and '%2' in field '%3' calculation !")
                .arg(PARAM_MAX_LEN, PARAM_MAX_LEN, CS_JSON_KEY_CALC_PARAMS);
    }
    return ok;
}

Splitter::Splitter() :
    _error(""),
    _result("")
{
}
