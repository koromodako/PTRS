#include "joiner.h"
#include "../../server/src/calculation/specs.h"
#include "bruteforce_specs.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariantMap>

bool Joiner::join(const QString &json)
{
    bool ok = false;

    // -- récupération des fragments
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {   _error  = "Failed to parse input json array in join !";
        return ok;
    }
    QJsonArray fragments = doc.array();

    // -- parsing fragments
    foreach (QJsonValue frag, fragments)
    {   QVariantMap params;
        if(frag.isObject())
        {   QJsonObject obj = frag.toObject();
            if(obj.contains(CS_JSON_KEY_CALC_RESULT) && obj.value(CS_JSON_KEY_CALC_RESULT).isObject())
            {   // récupération du champ résultat
                params = obj.value(CS_JSON_KEY_CALC_RESULT).toObject().toVariantMap();
                // vérification de la présence d'un match
                if(params.contains(PARAM_HAS_MATCH) && params.value(PARAM_HAS_MATCH).toBool())
                {   // on stock le fragment contenant le match dans l'attribut resultat
                    _result = QJsonDocument(obj).toJson(QJsonDocument::Compact);
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
