
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>

#include "datetypes.h"
#include "enviroment.h"
#include "exceptions.h"

using namespace std;

// **************************     Elem      **************************

//...

// **************************     LInt      **************************

string LInt::printable() {
    char buffer[20];
    sprintf(buffer, "%d", this->repr);
    return buffer;
}

string LInt::whois() {
    return "int";
}

Elem* LInt::copy() {
    LInt * newone = new LInt;

    newone->repr = this->repr;

    return newone;
}

LInt::LInt(string & tokRepr) {

    istringstream is(tokRepr);
    is >> this->repr;
}


// **************************     LSymbol      **************************

string LSymbol::printable() {
    //    string str = "Symbol: ";
    //    str.append(this->name);
    //    return str;
    return this->name;
}

string LSymbol::whois() {
    return "symbol";
}

Elem* LSymbol::copy() {
    LSymbol * newone = new LSymbol;

    newone->name = this->name;

    return newone;
}

LSymbol::LSymbol(string & tokRepr) {
    this->name = tokRepr;
}


// **************************     LList      **************************

LList::LList(LList& r) {
    LList * tmp = dynamic_cast<LList*> (r.copy());
    this->car = tmp->car;
    this->cdr = tmp->cdr;
}

Elem * LList::copy() {
    LList * newone = new LList;

    if (this->car) {
        newone->car = this->car->copy();
    } else {
        newone->car = NULL;
    }

    if (this->cdr) {
        newone->cdr = dynamic_cast<LList*> (this->cdr->copy());
    } else {
        newone->cdr = NULL;
    }
    return newone;
}

string LList::printable() {
    LList * elem = this;
    string s;
    s.append("(");
    while (elem) {
        if (elem->car) {
            s.append(elem->car->printable());
            if (elem->cdr) {
                s.append(" ");
            }
        }

        elem = elem->cdr;

    }
    s.append(")");

    return s;
}

string LList::whois() {
    return "list";
}

void LList::add(Elem * e) {
    LList * last = this->last();

    if (last) {
        LList * node = new LList();

        node->car = e;
        node->cdr = NULL;
        last->cdr = node;
    } else {
        this->car = e;
    }
}

int LList::len() {
    int len = 0;
    if (!this)
        return 0;
    
    if (this->car == NULL)
        return 0;
    
    LList * elem = this;
    while (elem) {
        len++;
        elem = elem->cdr;
    }
    return len;
}

LList * LList::last() {
    if (this->car == NULL) {
        return NULL; //tenhle je posledni
    }

    LList * last = this;

    while (last->cdr) {
        last = last->cdr;
    }

    return last;
}


// **************************     LString      **************************

string LString::printable() {
    string s;
    //    s.push_back('"');
    s.append(this->repr);
    //    s.push_back('"');
    return s;
}

string LString::whois() {
    return "string";
}

Elem* LString::copy() {
    LString * newone = new LString;

    newone->repr = this->repr;

    return newone;
}

LString::LString(string & tokRepr) {
    this->repr = tokRepr;
}

// **************************     LFunctionC      **************************
//LFunction::LFunction(Elem * (*funct)(Elem *)){
//    this->funct = funct;
//}

string LFunctionC::printable() {
    string s;
    s.append("Funct C");
    //    s.append(this->repr);
    //    s.push_back('');
    return s;
}

string LFunctionC::whois() {
    return "functionC";
}

Elem* LFunctionC::copy() {
    LFunctionC * newone = new LFunctionC(*this);

    return newone;
}

Elem * LFunctionC::call(LList * params, Enviroment * env) {

    Elem * returnValue = this->funct(params, env);

    return returnValue;
}

// **************************     LFunctionL      **************************

string LFunctionL::printable() {
    string s;
    s.append("Funct L");
    return s;
}

string LFunctionL::whois() {
    return "functionL";
}

Elem* LFunctionL::copy() {
    LFunctionL * newone = new LFunctionL;

    newone->funct = dynamic_cast<LList*> (this->funct->copy());
    newone->params = dynamic_cast<LList*> (this->params->copy());

    return newone;
}

Elem * LFunctionL::call(LList * params, Enviroment * env) {
    //    cout << "call given:" << params->len() << " declared: " << this->params->len() << endl;
    if (params->len() != this->params->len()) {
        TypeException e;
        e.desc = strformat("Bad number of parameters. %d taken (%d given)", this->params->len(), params->len());
        throw e;
    }

    Enviroment newEnv(*env); //volaní funkce probíhá v odděleném enviromentu

    LList * DeclaredParam = this->params;
    LList * GivenParam = params;

    while (DeclaredParam) { //dosazení skutečných parametrů za deklarované
        LSymbol * p = dynamic_cast<LSymbol*> (DeclaredParam->car);
        newEnv.define(p->name, GivenParam->car);

        DeclaredParam = DeclaredParam->cdr;
        GivenParam = GivenParam->cdr;
    }
    return newEnv.progn(funct);
}


// **************************     LFunctionM      **************************

string LFunctionM::printable() {
    string s;
    s.append("Macro: ");
    s.append(this->funct->printable());
    return s;
}

string LFunctionM::whois() {
    return "LFunctionM";
}

Elem* LFunctionM::copy() {
    LFunctionM * newone = new LFunctionM;

    newone->funct = dynamic_cast<LList*> (this->funct->copy());
    newone->params = dynamic_cast<LList*> (this->params->copy());

    return newone;
}

Elem * LFunctionM::call(LList * params, Enviroment * env) {
    int given, declared;
    if (params)
        given = params->len();
    else
        given = 0;

    if (this->params)
        declared = this->params->len();
    else
        declared = 0;

    if (given != declared) {
        TypeException e;
        e.desc = strformat("Bad number of parametres: %d given, %d expected", given, declared);
        throw e;
    }

    Enviroment newEnv(*env);

    LList * DeclaredParam = this->params;
    LList * GivenParam = params;

    if (GivenParam) {
        while (DeclaredParam) { // dosazení skutečných parametrů za deklarované
            LSymbol * p = dynamic_cast<LSymbol*> (DeclaredParam->car);
            newEnv.define(p->name, GivenParam->car);
            //        cout << "p: " << p->name << " " << GivenParam->car->printable() << endl;
            DeclaredParam = DeclaredParam->cdr;
            GivenParam = GivenParam->cdr;
        }
    }

    LList * f = dynamic_cast<LList*> (this->funct->copy());

    Elem * expanded = newEnv.progn(f); // expandované makro
    //    cout << "expanded macro: " << expanded->printable() << endl;

    return env->eval(expanded);
}


// **************************     LBool      **************************

LBool::LBool(bool val) {
    this->val = val;
}

string LBool::printable() {
    if (this->val) {
        return "True";
    } else {
        return "False";
    }
}

Elem* LBool::copy() {
    LBool * newone = new LBool(this->val);

    return newone;
}

string LBool::whois() {
    return "bool";
}


// **************************     HELPERS:

void Nparams(int n, LList * params) {
    if (params) {
        if (params->len() == n)
            return;
    } else
        if (0 == n)
        return;


    TypeException e;
    e.desc = strformat("Bad number of parametres: %d given, %d expected", params->len(), n);
    throw e;
}

template <typename T>
T * expect_X(Elem * x, string ident) {
    string type = x->whois();

    if (type == ident) {
        return dynamic_cast<T*> (x);
    }
    TypeException e;
    e.desc = strformat("Bad type: %s given, %s expected", type.c_str(), ident.c_str());
    throw e;
}

LBool * expect_LBool(Elem * x) {
    return expect_X<LBool > (x, "bool");
}

LInt * expect_LInt(Elem * x) {
    return expect_X<LInt > (x, "int");
}

LList * expect_LList(Elem * x) {
    return expect_X<LList > (x, "list");
}

LString * expect_LString(Elem * x) {
    return expect_X<LString > (x, "string");
}

LSymbol * expect_LSymbol(Elem * x) {
    return expect_X<LSymbol > (x, "symbol");
}

//LFunction * expect_Lfunction(Elem * x) {
//    return expect_X<LFunction > (x, "function");
//}

LFunctionC * expect_LfunctionC(Elem * x) {
    return expect_X<LFunctionC > (x, "functionC");
}

LFunctionL * expect_LfunctionL(Elem * x) {
    return expect_X<LFunctionL > (x, "functionL");
}

LFunctionM * expect_LfunctionM(Elem * x) {
    return expect_X<LFunctionM > (x, "functionM");
}
