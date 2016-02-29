# Guide de style pour les sources C++/Qt

Le guide de style à été conçu de sorte à être le moins contraignant possible (on est fainéants et c'est normal). 

Les objectifs de ce guide de style sont les suivants :
 + garantir l'homogénéité du code,
 + favoriser la lisibilité et la simplicité d'utilisation en surchargeant peu la notation,
 + utiliser le mecanisme d'autocomplétion plus efficacement.

Dans le fichier d'en-tête (.h)

        ```
#ifndef XXXX_H
#define XXXX_H

// -- includes
#include ...

// -- defines
#define CONSTANTE_NUMERIQUE 12                  // --> nom des constantes en UPPER_CASE et underscore

// -- typedefs & structs

// -- class

class Xxxx : [public Yyyy](si heritage)         // --> nom des classes en CamelCase
{
    Q_OBJECT
public:
    enum UnEnum {                               // --> nom des enum en CamelCase
        VALEUR_UN = 0x01,                       // --> nom des valeurs d'enum en UPPER_CASE et underscore (ce sont des constante un peu comme les constantes preprocesseur)
        ..
    };

    Xxxx();
    ~Xxxx(){}                                   // --> toujours mettre un destructeur permet de verifier qu'il n'a pas été oublié.

    void UneMethodePublique(int unArgument);   // --> methode publique CamelCase avec les arguments en CamelCase (! non préfixé comme les attributs cf. fin de ce fichier).

protected:
    /**
     * @brief UneMethodeProtegeeOuPrivee        // --> documentation doxygen en bloc précédent le prototype (pas de doc dans les .cpp)
     */
    void uneMethodeProtegeeOuPrivee();          // --> méthode protegee ou privee CamelCase avec une minuscule au debut                                 

public slots: 
	void Slot_unSlot();                        // --> un slot publique (spécial Qt) : CamelCase préfixé par 'Slot_'                               

private slots: 
	void slot_unAutreSlot();                        // --> un slot privé (spécial Qt) : CamelCase préfixé par 'slot_'

signals:
    void sig_unSignal();                            // --> un signal (spécial Qt) : CamelCase préfixé par 'sig_'

private:
    int _unAttributPrive;                     // --> un attribut (toujours privé #encapsulation, et pas de Setter sauf cas très particulier) préfixé par '_' et lowercase et underscore. 

};

#endif // XXXX_H
```

Dans le fichier source (.cpp)

        ```
#include "xxxx.h"

Xxxx::Xxxx() :
     _unAttributPrive(0) 
{
}

void Xxxx::UneMethodePublique(int unArgument) 
{
        int uneVariableLocale = 0;              // --> une variable locale en CamelCase et commençant par une minuscule. Comme les méthodes protégées et privées.
        uneVariableLocale++;
}

void Xxxx::SLOT_UN_SLOT()
{
}

void Xxxx::uneMethodeProtegeeOuPrivee()
{
}

```
