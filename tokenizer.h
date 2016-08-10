
#ifndef _TOKENIZER_H
#define	_TOKENIZER_H


#include <string>

#include "tokendefs.h"
#include "datetypes.h"

using namespace std;

/*!
 * @brief Převedení zdrojáku na tokeny
 * 
 * Třída Tokenizer slouží k "rozdrobení" zdrojáku na tokeny. Token je kousek
 * zdrojáku s informací o jeho typu (např. "143" - INT). Zdroják se předává
 * funkcí tokenizeString(), Tokeny se získávají fcí nextToken. Udržuje si 
 * informace v atributech line (zpracovávaný řádek) a col (pozice v řádku)
 * (vhodné pro debugging)
 */
class Tokenizer {
public:
    //! Implicitní konstruktor vynuluje vnitřní proměnné
    Tokenizer() {
        this->pos = 0;
        this->line = 1;
        this->col = 0;
    }

    /*! Konstruktor-like parametr který nastaví vnitřní stav objektu pro
     * tokenizaci předaného stringu
     * @param s string který se bude tokenizovat
     * */
    void tokenizeString(string s);

    /*! Funkce která se volá a která předává typ a textovou reprezentaci tokenu.
     * Po posledním vrátí token 0
     * @param[out] tokRepr - sem se uloží textová reprezentace tokenu
     * @return typ tokenu (definováno v tokendefs.h)
     * */
    token nextToken(string & tokRepr);

    //! Číslo řádku v zpracovávaném zdrojáku. Spolu s this->col se používá
    //! při chybových hlášeních
    int line;
    //! Pozice v rámci řádku. @sa this->line.
    int col;

private:
    //! skutečná funkce nextToken(). nextToken() jen předává práci této funkci (oddělené kvůli debuggingu).
    //! API je stejné jako nextToken()
    token nextToken2(string & tokRepr);

    //! Přesune ukazatel pozice na další kus kódu. Přeskakuje bílé znaky a komentáře (od # do konce rádku)
    void skipWhitespaces();

    //! Přečte a vrátí textovou reprezentaci čísla. Posune ukazatele pozice.
    string readNum();

    //! Přečte a vrátí textovou reprezentaci symbolu. Posune ukazatele pozice.
    string readSymbol();

    //! Přečte a vrátí textovou reprezentaci lispovského stringu. Posune ukazatele pozice.
    string readText();

    //! Přečte a vrátí jeden znak. A posune všecky ukazatele pozice.
    int getChar();

    //! Vrátí stav ukazatelů o jedno zavolání funkce getChar() zpět. Na konci zdrojáku vrátí -1
    void ungetChar();

    //! Pozice v aktuálně zpracovávaném řetězci (ukazuje na prvek ktery bude teprve cten)
    int pos;

    //! interní reprezentace zpracovávaného řetězce
    string repr;
};

#endif	/* _TOKENIZER_H */

