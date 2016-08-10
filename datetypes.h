
#ifndef _DATETYPES_H
#define	_DATETYPES_H

#include <string>

#include "enviroment.h"

class Enviroment;

using namespace std;

/*! @brief Abstraktní prataťka všech datových typů
 */
class Elem {
public:

    Elem() {
        this->line = 0;
        this->col = 0;
    }

    //! Vrátí tisknutelnou reprezentaci Elem.
    //! Je používáno při debuggingu a funkcí print
    virtual string printable() = 0;

    //! funkce whois() vrátí textový identifikátor datového typu. Používá se
    //! kvůli dynamickému typování lispu
    virtual string whois() = 0;
    
    virtual ~Elem()=0;
    
    //! Vrátí kopii objektu
    virtual Elem* copy() = 0;

    //! Číslo řádku (kvůli zobrazuje se v chybových hláškách)
    int line;
    //! Pozice na řádku @sa this->line
    int col;
};

//! Třída pro symboly (názvy proměnných a funkcí)

class LSymbol : public Elem {
public:

    LSymbol() {
    }

    //! Vytvoří nový LSymbol z jeho textové reprezentace (tokenu)
    LSymbol(string & tokRepr);

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    //! název symbolu (obsah je definován v Enviroment->symbols)
    string name;

private:
};

//! Celočíselný datový typ.

class LInt : public Elem {
public:

    LInt() {
        this->repr = 0;
    }

    LInt(const int num) {
        this->repr = num;
    }

    LInt(string & tokRepr);

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    //! Vnitřní reprezentace
    int repr;
private:
};

//! Seznam - linked list

class LList : public Elem {
public:


    LList(LList&);

    LList() {
        this->car = NULL;
        this->cdr = NULL;
    }

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    // přidá prvek na konec seznamu
    void add(Elem * e);

    //! vrátí délku seznamu
    int len();

    //! vrátí ukazatel na poslední prvek seznamu
    LList * last();

    //! obsah tohoto prvku @sa this->cdr
    Elem * car;

    //! odkaz na následující prvek @sa this->car
    LList * cdr;
};

//! Datový typ pro řetězce

class LString : public Elem {
public:

    LString() {
    }
    LString(string & tokRepr);

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    //! vnitřní reprezentace řetezce
    string repr;
private:
};

//! Společný datový typ pro primitivní a lispovské funkce a makra.

class LFunction : public Elem {
public:
    // všecky druhy funkcí se volají s stejným API:
    virtual Elem * call(LList * params, Enviroment *) = 0;

    //! Název přeřazený funkci (nepoviné, zobrazuje se v chybových hláškách)
    string name;
};

//! Primitivní funkce (v C)

class LFunctionC : public LFunction {
public:

    LFunctionC(Elem * (*funct)(LList *, Enviroment * env)) {
        //        cout << "added FC: " << funct << endl;
        this->funct = funct;
    }

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    Elem * call(LList * params, Enviroment *);

    //! Ukazatel na funkci v C (všecky mají stejné c-API)
    Elem * (*funct)(LList *, Enviroment *);
};

//! Lispovská funkce

class LFunctionL : public LFunction {
public:

    LFunctionL() {
    }

    LFunctionL(LList * params, LList * funct) {
        //        cout << "added FL: " << funct << "params: " << params->printable() << "funct: " << funct->printable() << endl;
        this->params = params;
        this->funct = funct;
    }

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    Elem * call(LList * params, Enviroment * env);

    //! seznam parametrů
    LList * params;

    //! seznam reprezentující tělo funkce
    LList * funct;
};

//! Lispovské makro

class LFunctionM : public LFunction {
public:

    LFunctionM() {
    }

    LFunctionM(LList * params, LList * funct) {
        //        cout << "added M: " << funct << "$ " << params->printable() << " $ " << funct->printable() << endl;
        this->params = params;
        this->funct = funct;
    }

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    Elem * call(LList * params, Enviroment * env);

    //! seznam parametrů
    LList * params;

    //! tělo makra
    LList * funct;
};

//! Datový typ pro logické hodnoty

class LBool : public Elem {
public:

    LBool() {
    }
    LBool(bool val);

    virtual string printable();
    virtual string whois();
    virtual Elem* copy();

    //! vnitřní reprezentace
    bool val;
};


// HELPERS:

/*! Pomocná funkce pro kontrolu počtu parametrů v primitivních funkcích.
 * @param n - požadovaný počet parametrů
 * @param params - parametry předané primitivní funkci
 * @return - nic nevrací, v případě že počty parametrů sobě neoodpovídají, vyvolá
 *           vyjímku TypeException
 */
void Nparams(int n, LList * params);

/*! šablona pro dynamickou kontrolu typů a přetypovávání
 * @param x - proměnná, jejíž typ se bude kontrolovat
 * @param ident - textový identifikátor požadovaného typu
 * @return ukazatel na správný typ (místo obecného Elem*)
 */
template <typename T>
T * expect_X(Elem * x, string ident);

/*! Bere jako parametr ukazatel na obecný typ Elem, a v případě že jecíl ukazatele
 * typu LBool tak vrátí přetypovaný ukazatel na tu hodnotu. Jinak vyhodí vyjímku
 * TypeException
 * @param x - ukazatel na obecný typ Elem *
 * @return - ukazatel na konkrétní typ LBool *
 */
LBool * expect_LBool(Elem * x);
LInt * expect_LInt(Elem * x); //!< @sa expect_LBool
LList * expect_LList(Elem * x); //!< @sa expect_LBool
LString * expect_LString(Elem * x); //!< @sa expect_LBool
LSymbol * expect_LSymbol(Elem * x); //!< @sa expect_LBool
//LFunction * expect_Lfunction(Elem * x);//!< @sa expect_LBool
LFunctionC * expect_LfunctionC(Elem * x); //!< @sa expect_LBool
LFunctionL * expect_LfunctionL(Elem * x); //!< @sa expect_LBool
LFunctionM * expect_LfunctionM(Elem * x); //!< @sa expect_LBool

#endif	/* _DATETYPES_H */

