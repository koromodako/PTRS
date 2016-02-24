# Projet Tutoré Réseau/Sécurité

## Concept

Développement d'une architecture collaborative de calcul distribué.

Les principales caractéristiques à respecter sont :
 - **extensibilité** : naturellement résolue par l'architecture clients-serveur (pas de boulot de ce côté là).
 - **hétérogénéité** : résolue par la mise en place du protocole de niveau application.
 - **accès concurrents** : mise en place d'un mécanisme de type MUTEX au niveau du serveur. (_pas prioritaire_)
 - **chiffrement** : mise en place du chiffrement des communications (SSL par exemple mais c'est _pas prioritaire non plus_)
 - **tolérance aux pannes** : partiellement résolue par l'archi. clients-serveur (sauf dans le cas ou c'est le serveur qui meurt) mais sinon on met en place au niveau du protocole applicatif un système de transactionnel.

### Protocole de communication (niveau applicatif)

Le protocole (**en cours de conception**) est le suivant :
 - client (C) --> serveur (S) :
   + **HELLO** : demande de connexion avec le serveur
   + **READY** *\<id>* : le client notifie le serveur qu'il est prêt à calculer pour lui en lui donnant son identifiant
   + **WORKING** *\<id>* : le client notifie le serveur qu'il a bien reçu le bloc de calcul et qu'il travaille.
   + **UNABLE** *\<id>* : le client notifie le serveur qu'il ne peut pas effectuer le calcul
   + **DONE** *\<id>* *\<calculation\_result\_block>* : le client notifie le serveur qu'il a terminé le calcul et renvoie le bloc résultat (sans doute une structure JSON générique pour un résultat de calcul)
   + **ABORT** *\<id>* : le client notifie le serveur qu'il a abandonné le calcul
 - S --> C :
   + **OK** [*\<id>*] : réponse positive, et si champ id présent : affectation d'un identifiant au client que ce dernier doit utiliser pour communiquer avec le serveur par la suite.
   + **KO** [*\<ip>* *\<port>*] : réponse négative, qui signifie, si les champs *\<ip>* et *\<port>* sont présents, va voir l'autre serveur, sinon reste en standby.
   + **DO** *\<calculation\_block>* : réponse à READY, donne un morceau de calcul au client *\<calculation_block>* sera sans doute une structure JSON générique pour un calcul
   + **STOP** : ordre donné au client d'arrêter son calcul

Un scénario de communication dans le cas nominal serait :
 - C > S : **HELLO**
 - S > C : ( **OK** *\<id>* | **KO** [*\<ip>* *\<port>*] )
 - C > S : **READY** *\<id>*
 - S > C : **DO** *\<calculation\_block>*
 - C > S : ( **WORKING** *\<id>* | **UNABLE** *\<id>* )
 - S > C : ( **OK** | **STOP** )
 - ... un certains temps s'écoule ...
 - C > S : ( **DONE** *\<id>* *\<calculation\_result\_block>* | **ABORT** *\<id>* )
 - S > C : **OK**

Concernant le client un graphe d'états lui aussi provisoire : cf graphes_etats.pdf

### Calculs

Cette section concerne les deux structures auxquelles font référence les champs *\<calculation\_block>* et *\<calculation\_result\_block>* présents dans les messages du protocole précédemment décrits.

Ces structures seraient spécifiées comme suit :

Spécification de la structure *\<calculation\_order\_block>* :
  ```json
{
  "bin":"<binary_basename>",
  "params":{
    <list_of_param_value_pairs>
  }
}
```

Spécification de la structure *\<calculation\_block>* :
  ```json
{
  "bin":"<binary_basename>",
  "fragment_id":"<fragment_id>",
  "params":{
    <list_of_param_value_pairs>
  }
}
```

Spécification de la structure *\<calculation\_result\_block>* :
  ```json
{
  "fragment_id":"<fragment_id>",
  "result":{
    <serialized_data_structure>
  }
}
```

Exemple pour un calcul de bruteforce :

*\<calculation\_order\_block>*
  ```json
{
  "bin":"bruteforce",
  "params":{
    "charset":"+-!?&=[a-zA-Z0-9]",
    "min_len":2,
    "max_len":15,
    "hash_func":"sha256",
    "target":"5E884898DA28047151D0E56F8DC6292773603D0D6AABBDD62A11EF721D1542D8"
  }
}
```
*\<calculation\_block>* :
  ```json
{
  "bin":"bruteforce",
  "fragment_id":"1",
  "params":{
    "charset":"+-!?&=[a-zA-Z0-9]",
    "min_len":6,
    "max_len":10,
    "hash_func":"sha256",
    "target":"5E884898DA28047151D0E56F8DC6292773603D0D6AABBDD62A11EF721D1542D8"
  }
}
```
*\<calculation\_result\_block>* :
  ```json
{
  "fragment_id":"1",
  "result":{
    "has_match":true,
    "match_string":"password"
  }
}
```
## Serveur

### CLI

Les commandes permettant d'agir sur le serveur :

 - **EXEC** *\<calculation\_order\_block>* : Demande l'execution du calcul au serveur
 - **STATUS** : Liste les calculs demandés au serveur, leurs identifiants respectifs et l'état de ces derniers (états d'un calcul : *READY*, *EXEC*, *DONE*, *CANCELLED*)
 - **RESULT** *\<calculation_id>* : retourne les résultats du calcul dont l'identifiant est passé en paramètre
 - **CANCEL** *\<calculation_id>* : demande l'annulation du calcul dont l'identifiant est passé en paramètre

Exemple de sorties pour les commandes :

  ```bash
server < EXEC {"bin":"bruteforce","params":{"charset":"+-!?&=[a-zA-Z0-9]","min_len":2,"max_len":15,"hash_func":"sha256","target":"5E884898DA28047151D0E56F8DC6292773603D0D6AABBDD62A11EF721D1542D8"}}
server > OK, calculation scheduled (calculation_id=1)
server < STATUS
server > 
+----------------+--------------------+----------------+-----------------+
| calculation_id | calculation_status | fragment_count | clients_working |
+----------------+--------------------+----------------+-----------------+
|        1       |       working      |      16/20     |      23/34      |
+----------------+--------------------+----------------+-----------------+
|        2       |        done        |      10/10     |       0/34      |
+----------------+--------------------+----------------+-----------------+
|        3       |      cancelled     |     42/200     |       0/34      |
+----------------+--------------------+----------------+-----------------+
server < CANCEL 1
server > OK, calculation cancelled (calculation_id=1)
server < RES
server > Unknown command.
server < RESULT
server > Missing parameter <calculation_id> for RESULT command.
server < RESULT 1
server > Calculation 1 is not done yet, 4 fragments are missing over 20.
server < RESULT 2
server > 
<serialized_data_structure>
server < _
```

Afin de garantir la qualité des résultats récupérés, le serveur se base sur un système de confiance en dupliquant les fragments de calcul et en distribuant plusieurs fois le même fragment à des clients différents comparant leurs résultats.

Un système d'authentification pour garantir l'identité des clients pourra également être mis en place.

### Système de plugins pour les calculs

L'idéal étant d'avoir un serveur générique pour effectuer tout type de calcul distribuable, un système de plugin a été mis en place. Nous avons donc la relation un calcul = un plugin = un binaire.

Ces plugins doivent être situés dans un dossier **plugins/** dans le même dossier que le binaire du serveur.

Ces plugins doivent offrir les fonctionnalités suivantes :
 + **-split \<json\_url\_encoded>** : qui permet de découpé un calcul en fragments parallèlisables
 + **-calc \<json\_url\_encoded>** : qui permet de réaliser le calcul pour un fragment en particulier
 + **-join \<json\_url\_encoded>** : qui permet de fusionner les résultats des fragments du calcul initial

Suite à ces appels les plugins peuvent réagir de deux manières différentes :
 + écrire dans la sortie standard le **résultat du traitement sous la forme d'un \<json\_url\_encoded>** si tout s'est déroulé comme prévu et terminer avec le **code de sortie égal à 0**,
 + écrire dans la sortie d'erreur un **message décrivant l'erreur** et terminer avec un **code de sortie différent de 0**

Nous envisageons de mettre en place un système de vérification d'intégrité des plugins basé sur un checksum MD5.

## Client

Les clients peuvent être implémentés dans n'importe quel langage (**hétérogénéité**). Ils doivent également implémenter les algorithmes nécessaires à l'exécution du calcul demandé.

## Mise en place de l'environnement de développement

Vous n'avez rien à configurer si vous utiliser l'IDE QtCreator.
Vous pouvez télécharger et installer Qt en suivant ce [lien](http://www.qt.io/download-open-source/).

