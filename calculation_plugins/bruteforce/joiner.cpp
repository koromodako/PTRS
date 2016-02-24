#include "joiner.h"
#include "../../server/src/calculation/specs.h"
#include "bruteforce_specs.h"
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>

bool Joiner::join(const QString &json)
{
    bool ok = false;

    // -- récupération des fragments
    QStringList fragments = json.split(CS_FRAGMENT_SEP);

    // -- parsing fragments
    foreach (QString json_frag, fragments)
    {   QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(json_frag.toUtf8(), &error);
        QVariantMap params;
        if(!error.error && doc.isObject())
        {   if(doc.object().contains(CS_JSON_KEY_CALC_RESULT) && doc.object().value(CS_JSON_KEY_CALC_RESULT).isObject())
            {   // récupération du champ résultat
                params = doc.object().value(CS_JSON_KEY_CALC_RESULT).toObject().toVariantMap();
                // vérification de la présence d'un match
                if(params.contains(PARAM_HAS_MATCH) && params.value(PARAM_HAS_MATCH).toBool())
                {   // on stock le fragment contenant le match dans l'attribut resultat
                    _result = json_frag;
                    // on lève le drapeau
                    ok = true;
                    // on a trouvé le résultat on interrompt le traitement
                    break;
                }
                // sinon on continue
            }
            else
            {   _error = QString("Missing '%1' field in calculation !")
                        .arg(CS_JSON_KEY_CALC_RESULT);
                // on interrompt sur erreur
                break;
            }
        }
        else
        {   _error = "Missing json object !";
            // on interrompt sur erreur
            break;
        }
    }
    return ok;
}

Joiner::Joiner() :
    _error(""),
    _result("")
{
}
