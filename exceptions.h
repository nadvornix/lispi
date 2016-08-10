#ifndef _EXCEPTIONS_H
#define	_EXCEPTIONS_H

#include <string>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <list>

#include "exceptions.h"
#include "datetypes.h"
//#include "enviroment.h"

using namespace std;

//! vytvoří string z format-stringu (jako u printf) a libovolného množství
//! parametrů které nesou hodnoty pro vložení.
//! Example: strformat("čísla %d a %d jsou %s", a, b, a==b?"stejná":"různá")
string strformat(const char* format, ...);

//! Rodičovská, abstraktní třída všech vyjímek
class BaseException {
public:

    string desc;    //!< Informace pro chybovou hlášku
    int line;       //!< Informace pro chybovou hlášku
    int col;        //!< Informace pro chybovou hlášku
    string file;    //!< Informace pro chybovou hlášku
    //! debug. info pro traceback
    list<LFunction*> functions; //!< Informace pro chybovou hlášku

    BaseException() {
        this->desc = "Unkown error";
        this->line = 0;
        this->col = 0;
        this->file = "";
    }

    void addFunction(LFunction * funct) {   //přidat funkci do tracebacku
        this->functions.push_back(funct);
    }
};

//! vyjímka pro špatné I/O operace (neexistující soubor atd)
class IOException : public BaseException {
};

//! vyjímka pro neexistující symbol (funkce i proměnné)
class NameException : public BaseException {
};

//! vyjímka pro chyby v syntaxi (vyhazuje Parser)
class SyntaxException : public BaseException {
};

//! vyjímka pro špatně použité datové typy (počet a typ parametrů primitivních funkcí)
class TypeException : public BaseException {
};

#endif	/* _EXCEPTIONS_H */
