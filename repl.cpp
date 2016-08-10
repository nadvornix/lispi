
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#include "repl.h"
#include "parser.h"
#include "exceptions.h"

void Repl::run(Enviroment env) {
    Parser p;

    while (cin.good()) {
        cout << "> ";   // prompt
        string command = this->getline(cin);

        LList* program = p.parseString(command);
        //        cout <<"program: " << program->printable();
        Elem * returned = evalProgram(&env, program);
        if (returned){
            cout << "# "<<returned->printable()<<endl;
        }
    }
}

string Repl::getline(istream&is) {
    string str;
    char chr = 0;
    while (is.good() && chr != '\n') {
        chr = is.get();
        str.push_back(chr);
    }

    return str;
}

//! Vytiskne Traceback (informace o stacku v době chyby)
void printTraceback(BaseException e) {
    cout << "---------------------------------------------------------------------------" << endl;
    cout << "Trackback:" << endl;
    for (list<LFunction*>::iterator it = e.functions.begin(); it != e.functions.end(); it++) {
        if ((*it)->name != "") {
            cout << "> " << (*it)->name << ": ";

            if ((*it)->line) {
                cout << "on line " << (*it)->line << " ";
            }
            cout << endl;
        }
    }
}

Elem * evalProgram(Enviroment * env, LList * program) {
    try {
        return env->progn(program);     // zkus program vykonat
    } catch (NameException e) {         // a kdyz se to nepovede napis co se stalo:

        if (!e.functions.empty()) {
            printTraceback(e);
        }
        cout << "NameError: " << e.desc << " ";
        if (e.line) {
            cout << "on line " << e.line << ". ";
        }

        cout << endl;
    } catch (BaseException e) { //vyjimky poskytuji v .desc dostatek informací.
        cout << "ERROR: " << e.desc << endl;
        if (!e.functions.empty()) {
            printTraceback(e);
        }
    }
    return NULL;
}

Elem * proccessFile(Enviroment& env, string filename) {
    Parser p;
    try {
        ifstream stdlibStream(filename.c_str(), ifstream::in);
        LList* stdlib = p.parseStream(stdlibStream);
        return evalProgram(&env, stdlib);
    } catch (IOException e) {
        cout << "lisp: Can't open file " << filename << ": No such file or directory" << endl;
        return NULL;
    } catch (SyntaxException e) {

        cout << e.desc << " in file " << filename << " on line " << e.line << endl;
    }
    return NULL;    //v případě chyby
}
