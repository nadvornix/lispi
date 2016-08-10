#ifndef _STDLIBC_H
#define	_STDLIBC_H

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "datetypes.h"
#include "enviroment.h"
#include "parser.h"
#include "repl.h"

LBool * True = new LBool(true);
LBool * False = new LBool(false);

/*! Vytiskne (a evaluuje) všechny své parametry.
 * <h4>Example:</h4> <pre>
 (print 1 2 (+ 1 1 1))
 </pre>
 */
Elem * printc(LList *params, Enviroment * env) {
    while (params) {
        cout << env->eval(params->car)->printable();
        params = params->cdr;
    }
    return False;
}

/*! vezme první parametr - požadovaný název (symbol) a pod tímto názvem uloží evaluovanou
 * hodnotu druhého parametru
 *
 * <h4>Example:</h4> <pre>
 (set deset (* 2 5))
 </pre>
 */
Elem * setc(LList *params, Enviroment * env) {
    Nparams(2, params);
    LSymbol * symbol = expect_LSymbol(params->car);

    Elem * value = env->eval(params->cdr->car);

    string whois = value->whois();
    if (whois == "functionC" || whois == "functionL" || whois == "LFunctionM") {
        LFunction * f = dynamic_cast<LFunction*> (value);
        f->name = symbol->name;
    }

    env->define(symbol->name, value);
    return value;
}

/*! Vezme svůj parametr a vrátí ho bez evaluace. Obdoba '(...) z Common Lispu
 * <h4>Example:</h4> <pre>
 (print (quote (1 2)))
 </pre>
 */
Elem * quotec(LList *params, Enviroment * env) {
    return params->car;
}

/*! Vytvoří anonymní funkci.
 * <h4>Example:</h4> <pre>
 (print ((lambda (x) (+ x 1)) 1)
 </pre>
 */
Elem * lambdac(LList *params, Enviroment * env) {
    Nparams(2, params);

    LList * newFparams = expect_LList(params->car);
    //    cout << "params: "<<newFparams->printable()<<endl;
    if (newFparams->car == NULL && newFparams->cdr == NULL) {
        newFparams = NULL;
    }
    LList * newFbody = expect_LList(params->cdr);

    LFunctionL * newFunct = new LFunctionL(newFparams, newFbody);


    newFunct->line = params->line;
    newFunct->col = params->col;

    return newFunct;
}

/*! Vytvoří makro. První parametr je název makra, druhý seznam parametrů, třetí tělo.
 * <h4>Example:</h4> <pre>
 (defmacro foo () (quote (print 2)))
 (foo)
 </pre>
 */
Elem * defmacroc(LList *params, Enviroment * env) {
    Nparams(3, params);

    LSymbol * symbol = expect_LSymbol(params->car);

    LList * newFparams = expect_LList(params->cdr->car);
    LList * newFbody = expect_LList(params->cdr->cdr);

    LFunctionM * newMacro = new LFunctionM(newFparams, newFbody);

    env->define(symbol->name, newMacro);

    return newMacro;
}

/*! Vrátí seznam utvořený z evaluovaných parametrů
 * <h4>Example:</h4> <pre>
 (print (list 1 2 3 (+ 2 2)))
 </pre>
 */
Elem * listc(LList *params, Enviroment * env) {
    LList *l = new LList;
    while (params){
        l->add(env->eval(params->car));
        params=params->cdr;
    }

    return l;
}

//Elem * letc(LList *params, Enviroment * env) {
//
//    //    symbol = (dynamic_cast<LSymbol*> (this->eval(params->head())))->name;
//    LSymbol * symbol = dynamic_cast<LSymbol*> (env->eval(params->car));
//
//    Elem * value = env->eval(params->cdr->car);
//
//    //    cout << "symname: " << symbol << endl;
//
//    //                this->symbols[symbol->name] = value;
//    env->define(symbol->name, value);
//    return value;
//}

//Elem * str2symbolc(LList *params, Enviroment * env) {
//    LString * lstr = dynamic_cast<LString*> (params->car);
//    LSymbol * sym = new LSymbol(lstr->repr);
//    return sym;
//}

/*! Obdoba `(...) v commonlispu.
 * Plní podobnou funkci jako (quote) akorát elementy které jsou v (comma (..)) se evaluují
 * a elementy v (at (...)) se vloží přímo.
 * Na rozdíl od CL se symboly at a comma nehledají rekurzivně.
 * <h4>Example:</h4>
 <pre>
   (set P (list 6 7))
   (print (backquote (1         # neevaluuje
                      (- 2 0)   # neevaluuje
                      3         # neevaluuje
                      (comma (+ 2 2))   # evaluuje
                      5         # neevaluuje
                      (at P)))) # evaluuje + se připojí
 </pre>
 * vypíše:
 * (1 (- 2 0) 3 4 5 6 7)
 */
Elem * backquotec(LList *params, Enviroment * env) {
    if (params->car->whois() != "list") {
        return params->car;
    }

    LList *p = expect_LList(params->car);
    LList *prev = NULL;

    while (p) {
        if (p->car->whois() == "list") {
            LList * tmp = expect_LList(p->car);
            if (tmp->car->whois() == "symbol") {
                if (expect_LSymbol(tmp->car)->name == "comma") {
                    p->car = env->eval(tmp->cdr->car);
                }
                if (expect_LSymbol(tmp->car)->name == "at") {
                    //                    p->car = env->eval(tmp->cdr->car);

                    LList * evaluated = expect_LList(env->eval(tmp->cdr->car));

                        if (prev) {
                            prev->cdr = evaluated;
                        }

                        LList *last = evaluated->last();
                        last->cdr = p->cdr;
                }
            }
        } else {
            //do nothing
        }
        prev = p;
        p = p->cdr;
    }
    //        cout << "backquote: " << params->car->printable() << endl;
    return params->car;
}

/*! podmínka IF. Syntaxe (if (podminka) (true-vetev) (false-vetev))
 * <h4>Example:</h4>
 <pre>
    (defun velikost (x y)
        (if (== x y)
            (print "jsou stejné")
            (if (> x y)
                (print "prvni je vetsi")
                (print "druhy je vetsi"))))
    (velikost 5 9)
    </pre>
 *  */
Elem * ifc(LList *params, Enviroment * env) {

    LBool * cond = expect_LBool(env->eval(params->car));

    if (cond->val==true){
        return  env->eval(params->cdr->car);
    }
    else {
        return env->eval(params->cdr->cdr->car);
    }
}

/*! Vyhodnotí výraz (předaný jako seznam)
 * <h4>Example:</h4> <pre> (eval  (print "nazdar!"))</pre>
 */
Elem * evalc(LList *params, Enviroment * env) {
    cout << "eval: "<<params->car->printable()<<endl;
    return env->eval(params->car);
}

/*! Vyhodnotí všecky své parametry. Vhodné na místa kam by se jinak dal napsat
 * jen jeden příkaz (např. tělo (lambda). Vrátí hodnotu posledního příkazu.
 * <h4>Example:</h4>
 <pre>
    (evall (print "Hello ")
           (print "World!")
           (print NL))
 </pre>
 */
Elem * evallc(LList *params, Enviroment * env) {
    Elem * result = env->progn(params);

    return result;
}

/*! Podobný jako evallc, akorát parametr vyhodnocuje 2x takže tam může být i proměná
 */
Elem * prognc(LList *params, Enviroment * env) {
    LList * tmp = expect_LList (env->eval(params->car));

    Elem * result = env->progn(tmp);

    return result;
}

/*! Klasická funkce CAR. Vrátí první prvek seznamu
 * <h4>Example:</h4>
 <pre>
    (set baz (list 1 2 3 4))
    (print (cdr baz))
    </pre>
 */
Elem * carc(LList *params, Enviroment * env) {
    return expect_LList(env->eval(params->car))->car;
}

/*! Klasická funkce CDR. Vrátí všechny kromě prvého prvku seznamu
 * <h4>Example:</h4>
 <pre>
    (set baz (list 1 2 3 4))
    (print (cdr baz))
    </pre>
 */
Elem * cdrc(LList *params, Enviroment * env) {
    return expect_LList(env->eval(params->car))->cdr;
}

/*! Vezme program v podobě stringu a rozparsuje ho na lispovský datový typy
 * <h4>Example:</h4>
 <pre>
    (set data "(1 1 2 3 5 8 13)")
    (print (parse data))
    </pre>
 */
Elem * parsec(LList *params, Enviroment * env) {
    Parser p;
    LString * lstr = expect_LString (env->eval(params->car));
    LList* program = p.parseString(lstr->repr);
    return program;
}

/*! Načte obsah souboru do LStringu.
 * <h4>Example:</h4>
 <pre>
    (set passwd (readfile "/etc/passwd"))
    (print passwd)</pre>
 */
Elem * readfilec(LList *params, Enviroment * env) {
    LString *filename = expect_LString (env->eval(params->car));
    ifstream ifs(filename->repr.c_str(), ifstream::in);
    string content;

    while (ifs.good()) {
        char c = ifs.get();
        if (ifs.good()) {
            content.push_back(c);
        }
    }

    //    cout << content;

    ifs.close();
    LString * lstr = new LString(content);

    return lstr;
}

/*! Vypíše všecky proměný v aktuálním prostředí
 * <h4>Example:</h4>
 <pre>
    (vardump)</pre>
 */
Elem * vardumpc(LList *params, Enviroment * env) {
    env->vardump();
    return NULL;
}

/*! Exportuje Všechny místní proměný do nadřazených prostředí.
 * (Pro interní použití v funkci import)
 */
Elem * exportallc(LList *params, Enviroment * env) {
    Enviroment * parent = env->parent;
    map<string, Elem *>::iterator it;
    while (parent) {
        for (it = env->symbols.begin(); it != env->symbols.end(); it++) {
            //            cout << (*it).first << endl;
            parent->define((*it).first, (*it).second);
        }

        env = parent;
        parent = parent->parent;
    }
    //    env->vardump();
    return NULL;
}

/*! Spustí REPL (interaktivní režim) v současném prostředí - vhodné pro debugging
 * <h4>Example:</h4>
 <pre>
    (set foo "ORLY?!")
    (repl)</pre>
 * Následně: <pre>(printnl foo)</pre>
 */
Elem * replc(LList *params, Enviroment * env) {
    Repl REPL;
    REPL.run(*env);
    return NULL;
}

/*! Sestaví lispovský seznam z svých dvou parametrů.
 * <h4>Example:</h4>
 <pre>
   (print (cons (cons 1 "dva") 3))</pre>
 */
Elem * consc(LList *params, Enviroment * env) {
    if (params->len() != 2) {
        cout << "cons: spatny pocet parametru" << endl;
    }
    Elem * a = env->eval(params->car);
    Elem * b = env->eval(params->cdr->car);

    LList * list = new LList;
    list->add(a);
    list->add(b);

    return list;
}


// ************** ARITHMETIC:

/*! Plus (+). <h4>Example:</h4><pre>(+ 1 2 3)</pre>
 */
Elem * plusc(LList *params, Enviroment * env) {
    LInt * result = new LInt();

    while (params) {
        LInt* val = expect_LInt (env->eval(params->car));
        result->repr += val->repr;

        params = params->cdr;
    }


    return result;
}

/*! Minus (-). <h4>Example:</h4> <pre>(- 128 64 32 16)</pre>
 */
Elem * minusc(LList *params, Enviroment * env) {
    LInt* val = expect_LInt (env->eval(params->car));

    if (params->len() == 1) { // unary minus
        return new LInt(-val->repr);
    }

    LInt * result = new LInt(val->repr);
    params = params->cdr;

    while (params) {
        val = expect_LInt (env->eval(params->car));
        result->repr -= val->repr;

        params = params->cdr;
    }

    return result;
}

/*! Násobení (*)
 */
Elem * multiplyc(LList *params, Enviroment * env) {
    LInt * result = new LInt(1);

    while (params) {
        LInt* val = expect_LInt (env->eval(params->car));
        result->repr *= val->repr;

        params = params->cdr;
    }


    return result;
}

/*! Celočíselné dělení (/). <h4>Example:</h4><pre> (print (/ 1000 10 2))</pre>
 */
Elem * divisionc(LList *params, Enviroment * env) {
    LInt* val = expect_LInt (env->eval(params->car));

    LInt * result = new LInt(val->repr);
    params = params->cdr;

    while (params) {
        val = expect_LInt (env->eval(params->car));
        result->repr /= val->repr;

        params = params->cdr;
    }

    return result;
}

/*! Zbytek po celočíselném dělení (%). <h4>Example:</h4><pre> (print (% 124 10))</pre> vytiskne 4
 */
Elem * moduloc(LList *params, Enviroment * env) {
    LInt* val = expect_LInt (env->eval(params->car));

    LInt * result = new LInt(val->repr);
    params = params->cdr;

    while (params) {
        val = expect_LInt (env->eval(params->car));
        result->repr %= val->repr;

        params = params->cdr;
    }

    return result;
}

// ************** LOGIC:

/*! Pomocná funkce pro zjišťování jestli se dva prvky rovnají.
 */
bool lequeal(Elem * a, Elem *b) {
    string typeA = a->whois();
    string typeB = b->whois();

    if (typeA.compare(typeB) != 0) { // nejsou stejne
        TypeException e;
        e.line = a->line;
        e.desc = strformat("Function == needs all parameters with same type. (%s vs. %s)",
                typeA.c_str(), typeB.c_str());

        throw e;
    }

    bool result;
    if (typeA == "int") {
        result = ((expect_LInt(a)->repr) == (expect_LInt(b)->repr));
    }

    if (typeA == "symbol") {
        result = false;
    }

    if (typeA == "list") {
        LList * LA = expect_LList(a);
        LList * LB = expect_LList(b);

        if (LA->len() != LB->len()) {
            return false;
        }

        while (LA) {
            try {
                if (!lequeal(LA->car, LB->car)) {
                    return false;
                }
            } catch (TypeException) {
                // sem se to dostane kdyz pri porovnavani seznamu narazime na stejny pozici na
                // prvky s jinym typem: je to OK => bez vyjimky, a seznamy jsou rozdilne
                return false;
            }
            LA = LA->cdr;
            LB = LB->cdr;
        }
        result = true;
    }

    if (typeA == "string") {
        LString* SA = expect_LString(a);
        LString* SB = expect_LString(b);
        result = SA->repr == SB->repr;
    }

    if (typeA == "functionC") {
        result = a == b;
    }

    if (typeA == "functionL") {
        result = a == b;
    }
    if (typeA == "LFunctionM") {
        result = a == b;
    }

    if (typeA == "bool") {
        result = ((dynamic_cast<LBool*> (a)->val) == (dynamic_cast<LBool*> (b)->val));
    }

    return result;
}

/*! Porovnávání obsahu několika prvků.
 * <h4>Example:</h4>
 <pre>
    (print (== 12 (- 15 3) (* 2 6) (+ 9 3) (/ 24 2) (% 52 20))</pre>
 * vypíše True.
 */
Elem * equalc(LList *params, Enviroment * env) {
    LBool * result = new LBool();

    string wantedType = params->car->whois();

    LList * a = params;
    LList * b = params->cdr;
    while (b) {
        if (!lequeal(env->eval(a->car), env->eval(b->car))) {
            result->val = false;
            return result;
        }

        a = a->cdr;
        b = b->cdr;
    }

    result->val = true;
    return result;
}

/*! Větší než. <h4>Example:</h4> <pre> (> 99 7 6 5 3)</pre>
 */
Elem * biggerc(LList *params, Enviroment * env) { //  >
    LBool * result = new LBool();

    LList * a = params;
    LList * b = params->cdr;
    while (b) {
        LInt* IA = expect_LInt(env->eval(a->car));
        LInt* IB = expect_LInt(env->eval(b->car));

        if (!(IA->repr > IB->repr)) {
            result->val = false;
            return result;
        }

        a = a->cdr;
        b = b->cdr;
    }

    result->val = true;
    return result;
}

/*! Menší než. <h4>Example:</h4> <pre> (< 99 7 6 5 3)</pre>
 */
Elem * smallerc(LList *params, Enviroment * env) {//  <
    LBool * result = new LBool();

    LList * a = params;
    LList * b = params->cdr;
    while (b) {
        LInt* IA = expect_LInt(env->eval(a->car));
        LInt* IB = expect_LInt(env->eval(b->car));

        if (!(IA->repr < IB->repr)) {
            result->val = false;
            return result;
        }

        a = a->cdr;
        b = b->cdr;
    }

    result->val = true;
    return result;
}

/*! Not - negace <h4>Example:</h4>
<pre>
    (defun >= (x y)
        (not (< x y)))
        </pre>
 */
Elem * notc(LList *params, Enviroment * env) {
    Nparams(1, params);
    LBool * result = expect_LBool(env->eval(params->car));
    result->val = !result->val;
    return result;
}

/*! AND - logický součin. Provádí zkrácené vyhodnocení.
 * <h4>Example:</h4>
 <pre>
    (and True (== 1 0) (print "nic se nevypíše"))
    </pre>
 */
Elem * andc(LList *params, Enviroment * env) {
    LBool * result = new LBool(true);

    while (params) {
        LBool * val = expect_LBool(env->eval(params->car));
        result->val = result->val && val->val;
        if (result->val == false) {
            return result;
        }

        params = params->cdr;
    }

    result->val = true;
    return result;
}

/*! OR - logický součet. Provádí zkrácené vyhodnocování.
 <h4>Example:</h4>
 <pre>
    (defun xor (x y)
        (or
            (and x (not y))
            (and (not x) y)))

    (print (xor False True))
    </pre>
 */
Elem * orc(LList *params, Enviroment * env) {
    LBool * result = new LBool(false);

    while (params) {
        LBool * val = expect_LBool(env->eval(params->car));
        result->val = result->val || val->val;
        if (result->val == false) {
            return result;
        }

        params = params->cdr;
    }

    result->val = true;
    return result;
}

//LFunction * test = new LFunction(testc);

#endif	/* _STDLIBC_H */

