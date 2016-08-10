
#ifndef _REPL_H
#define	_REPL_H

#include <string>
#include <iostream>

#include "enviroment.h"
#include "exceptions.h"
#include "parser.h"

using namespace std;

/*! Spustí lispovský program předaný jako strom príkazů a zachytí a zobrazí
 *  user-friendly informace o případných chybách
 * @param env - enviroment ve kterém proběhne spuštění
 * @param program spuštěný program
 * @return Buď návratovou hodnotu posledního příkazu nebo, v případě chyby, NULL
*/
Elem * evalProgram(Enviroment * env, LList * program);

//! Spustí lispovský program daný parametrem filename v enviromentu env.
Elem * proccessFile(Enviroment& env, string filename);

//! Repl - interaktivní interpret lispu
class Repl {
public:
    /*! Spustí REPL
     * @param env - enviroment ve kterém REPL poběží
     */
    void run(Enviroment env);
private:
    //! načte a vrátí řádek načtený z parametru is
    string getline(istream&is);
    //    void printExceptionInfo(BaseException e) ;
};

#endif	/* _REPL_H */

