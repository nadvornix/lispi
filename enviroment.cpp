
#include <map>
#include <string>

#include <iostream>

#include "datetypes.h"
#include "enviroment.h"
#include "stdlibc.h"
#include "exceptions.h"

using namespace std;

Enviroment::Enviroment(Enviroment& r) {
    this->parent = &r;
    this->symbols = r.symbols;
}

Enviroment::Enviroment() {
    this->parent = NULL;

    this->symbols["print"] = new LFunctionC(printc);
    this->symbols["set"] = new LFunctionC(setc);
    this->symbols["quote"] = new LFunctionC(quotec);
    this->symbols["lambda"] = new LFunctionC(lambdac);
    this->symbols["defmacro"] = new LFunctionC(defmacroc);
    //    this->symbols["let"]  = new LFunctionC(letc);
    //    this->symbols["str2symbol"] = new LFunctionC(str2symbolc);
    this->symbols["eval"] = new LFunctionC(evalc);
    this->symbols["progn"] = new LFunctionC(prognc);
    this->symbols["car"] = new LFunctionC(carc);
    this->symbols["cdr"] = new LFunctionC(cdrc);
    this->symbols["backquote"] = new LFunctionC(backquotec);
    this->symbols["list"] = new LFunctionC(listc);
    this->symbols["parse"] = new LFunctionC(parsec);
    this->symbols["readfile"] = new LFunctionC(readfilec);
    this->symbols["vardump"] = new LFunctionC(vardumpc);
    this->symbols["exportall"] = new LFunctionC(exportallc);
    this->symbols["evall"] = new LFunctionC(evallc);
    this->symbols["repl"] = new LFunctionC(replc);
    this->symbols["cons"] = new LFunctionC(consc);
    this->symbols["if"] = new LFunctionC(ifc);

    this->symbols["+"] = new LFunctionC(plusc);
    this->symbols["-"] = new LFunctionC(minusc);
    this->symbols["*"] = new LFunctionC(multiplyc);
    this->symbols["/"] = new LFunctionC(divisionc);
    this->symbols["%"] = new LFunctionC(moduloc);
    this->symbols["=="] = new LFunctionC(equalc);
    this->symbols[">"] = new LFunctionC(biggerc);
    this->symbols["<"] = new LFunctionC(smallerc);
    this->symbols["not"] = new LFunctionC(notc);
    this->symbols["and"] = new LFunctionC(andc);
    this->symbols["or"] = new LFunctionC(orc);

}

void Enviroment::define(string name, Elem * val) {
    this->symbols[name] = val;
}

void Enviroment::vardump() {
    map<string, Elem *>::iterator it;
    for (it = this->symbols.begin(); it != this->symbols.end(); it++) {
        string key = (*it).first;
        Elem * val = (*it).second;
        if (val) {
            cout << key << " => " << val << "      [" << val->printable() << "]" << endl;
        } else {
            cout << key << " => " << val << "      [NULL]" << endl;
        }
    }
}

Elem* Enviroment::progn(LList* expr) {
    Elem *pe=NULL;

    while (expr) {
        //        cout << "> " << expr->printable() << ":" << endl;
        pe = this->eval(expr->car);

        expr = expr->cdr;
    }

    return pe;
}

LList* Enviroment::evalAll(LList* expr) {
    if (expr == NULL) {
        return NULL;
    }
    LList *pe = new LList;

    //        cout <<endl<<"from:"<<expr->printable()<<endl;
    while (expr) {
        pe->add(this->eval(expr->car));
        expr = expr->cdr;
    }
    return pe;
}

Elem* Enviroment::eval(Elem* expr) {
    if (expr == NULL) {
        return NULL;
    }

    string type = expr->whois();
    //    cout << "evaluating: " << expr << " (" << type << ") " << expr->printable() << endl;

    if (type == "symbol") {
        LSymbol*sym = dynamic_cast<LSymbol*> (expr);

        Elem * val = this->symbols[sym->name];
        if (val == NULL) {  // neexistující proměná/funkce
            NameException e;
            e.line = sym->line;
            e.desc = string("Name ") += sym->name += " not defined";
            throw e;
        }

        return val;
    } else if (type == "int") {
        return expr;
    } else if (type == "string") {
        return expr;
    } else if (type == "list") {

        LList*list = dynamic_cast<LList*> (expr);

        Elem * head = list->car; //prvni prvek seznamu
        LList * params = list->cdr; //zbytek

//        if (head == NULL && params == NULL) {   // ()
//            return NULL;
//        }

        LFunction * f = dynamic_cast<LFunction*> (this->eval(head));    //prvni prvek seznamu je funkce

        string whois = f->whois();
        try {
            if (whois == "functionC") {
                return f->call(params, this);
            } else if (whois == "functionL") {
                LList * evaluatedParams = this->evalAll(params);
                return f->call(evaluatedParams, this);
            } else if (whois == "LFunctionM") {
                return f->call(params, this);
            }
        } catch (NameException e) { // V případě že se během volání stala chyba,
                                    // tak se tato funkce přidá do tracebacku
            e.addFunction(f);
            throw e;
        }
    }

    //sem by to nemelo dojit (?) TODO:
    cout << "ERROR: Uuaaaaaaaaa!!" << endl;
    return NULL;
}
