
#include <string>
#include <iostream>

#include "tokendefs.h"
#include "datetypes.h"
#include "tokenizer.h"
#include "parser.h"
#include "exceptions.h"

using namespace std;

Parser::Parser() : tokenizer() {
}

LList* Parser::parseStream(istream & stream) { //predela stream na string a pak zavolá parseString
    string str;

    stream.seekg(0);
    int length = stream.tellg();
    if (length == -1) { // spatny stream (neexistujici soubor)
        throw IOException();
    }

    stream.seekg(0);

    str.reserve(length);

    while (!stream.eof()) {
        str += stream.get();
    }

    return this->parseString(str);
}

LList* Parser::parseString(string & s) {
    this->tokenizer.tokenizeString(s);

    LList *l = this->n_List(s); // celej zdroják je seznam příkazů
    return l;
}

Elem * Parser::parseElem() {
    Elem * e = NULL;

    string tokRepr;

    token tok = this->tokenizer.nextToken(tokRepr);

    switch (tok) {
        case LIST_BEGIN: // zacatek noveho expr. = leva zavorka
            //                e = LList(tokRepr);

            e = this->n_List(tokRepr);

            //                if (this->tokenizer.nextToken(tokRepr) != LIST_END) {
            //                    cout <<"NEUZAVRENA ZAVORKA"<<endl;
            //                    return NULL; //TODO: throw neuzavrena zavorka
            //                }else {
            //                    cout <<"UZAVRENA Z"<<endl;
            //                    break;
            //                }
            break;

        case STRING_BEGIN:
            e = this->n_String(tokRepr);
            break;

        case INT:
            e = this->n_Int(tokRepr);
            break;


        case SYMBOL:
            e = this->n_Symbol(tokRepr);
            break;

        case LIST_END:
            return NULL;

        case 0:
            return NULL;
    }

    return e;
}

LList * Parser::n_List(string & tokRepr) { //vrati list s elementama; necheckuje zavorky
    LList *l = new LList;
    Elem * e;
    l->line = this->tokenizer.line;
    l->col = this->tokenizer.col;

    while ((e = this->parseElem())) {
        l->add(e);
    }

    return l;
}

LInt * Parser::n_Int(string & tokRepr) {
    LInt * li = new LInt(tokRepr);
    li->line = this->tokenizer.line;
    li->col = this->tokenizer.col;

    return li;
}

LString * Parser::n_String(string & tokRepr) {
    LString * lstr = new LString(tokRepr);
    lstr->line = this->tokenizer.line;
    lstr->col = this->tokenizer.col;

    return lstr;
}

LSymbol * Parser::n_Symbol(string & tokRepr) {
    LSymbol * ls = new LSymbol(tokRepr);
    ls->line = this->tokenizer.line;
    ls->col = this->tokenizer.col;

    return ls;
}
