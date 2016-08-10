#ifndef _PARSER_H
#define	_PARSER_H

#include <string>

#include "tokendefs.h"

#include "datetypes.h"
#include "tokenizer.h"

using namespace std;

//!Převede zdroják na programový strom
class Parser {
public:
    //! Inicializuje prázdný parser. Data k rozparsování se předávají metodama parseX()
    Parser();

    //! Převede předaný stream na string a předá parseString() (mají stejné API). @sa parseString()
    LList* parseStream(istream & stream);

    /*! Převede ředaný string na strom reprezentující program
     * @param s - textový řetěřec k rozparsování
     * @return - strom reprezentující program
     */
    LList* parseString(string & s);

private:
    Tokenizer tokenizer;

    //! Vezme další token a převede ho na Elem
    Elem * parseElem();

    //! Vezme token typu LIST a převede ho na lispovský LList
    LList * n_List(string & tokRepr);

    //! Vezme token typu INT a převede ho na token typu LInt
    LInt * n_Int(string & tokRepr);

    //! Vezme token typu STRING a převede ho na token typu LString
    LString * n_String(string & tokRepr);

    //! Vezme token typu SYMBOL a převede ho na token typu LSymbol
    LSymbol * n_Symbol(string & tokRepr);

};


#endif	/* _PARSER_H */

