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
    }
    // vérification des paramètres
    if(params.contains(PARAM_CHARSET) &&
       params.contains(PARAM_MIN_LEN) &&
       params.contains(PARAM_MAX_LEN))
    {   // récupération des paramètres pour le calcul
        quint64 charset_size = params.value(PARAM_CHARSET).toString().length();
        quint64 min_len = params.value(PARAM_MIN_LEN).toULongLong();
        quint64 max_len = params.value(PARAM_MAX_LEN).toULongLong();
        // calcul de la charge optimale
        quint64 max = AVG_TEST_PER_SEC*OPTIMAL_COMPUTATION_TIME;
        // calcul du nombre de fragments
        quint64 fcount(0);
        for (uint l = min_len; l <= max_len; ++l) {
            if(l > 2 && charset_size > 64)
            {   std::cerr << "Warning overflow risk on unsigned long long int." << std::endl;
            }
            fcount += pow(l,charset_size)/max;
        }
        //
        /// \todo calculer le resultat de la fragementation et l'exporter en JSON dans la variable de classe _result
        // on lève le drapeau
        ok = true;
    }
    return ok;
}

Splitter::Splitter(){}
