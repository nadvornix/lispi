
#ifndef _ENVIROMENT_H
#define	_ENVIROMENT_H

#include <map>
#include <string>
#include <iostream>

#include "datetypes.h"

class Elem;
class LList;
class LSymbol;
class LFunctionC;

using namespace std;

//! Enviroment - prostředí ve kterém se provádí kód
class Enviroment {
public:
    //! Implicitní konstuktor - vytvoří prázdný enviroment a definuje primitivní symboly
    Enviroment();

    //! Korektně fungující kopírující konstruktor.
    Enviroment(Enviroment&);

    //! definuje symbol (proměnou)
    void define(string name, Elem * val);

    //! Vypíše všecky definované symboly v aktuálním enviromentu
    void vardump();

    /*! evalAll - vyhodnotí parametry volání jen lispovské funkce (makra a 
     * primitivní funkce se evaluují jinak). Používá se kvůli call-by-value.
     * @param expr - parametry funkce
     * @return nový seznam kde je každý prvek z expr evaluován
     */
    LList* evalAll(LList* expr);

    /*! vyhodnotí seznam (+ 1 2)
     * Pravidla vyhodnocování: int a string se vyhodnocují na sebe.
     *  symbol se vyhodnocuje na co představuje (pomocí tabulky symbolů this->symbols)
     *  U seznamu se předpokládá že to je volání funkce. Jestli je to lispovská 
     * funkce tak se evaluují parametry a funkce se zavolá. U primitivních funkcí 
     * (v C) a maker se parametry neevalují.
     */
    Elem* eval(Elem* expr);

    //! Vyhodnotí seznam seznamů ((+ 1 2) (* 5 6))
    //! @return hodnotu posledního výrazu
    Elem* progn(LList* expr);

    //    Elem* eval(LSymbol *expr) ;
    //
    //    Elem* eval(LInt *expr) ;
    //
    //    Elem* eval(LString *expr);
    //
    //    Elem* eval(LList *expr);

    //! Tabulka definovaných symbolů
    map<string, Elem *> symbols;

    //! Odkaz na nadřazený enviroment (kvůli fci exportall)
    Enviroment * parent;
};

#endif	/* _ENVIROMENT_H */

