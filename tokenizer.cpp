
#include <string>
#include <iostream>

#include "tokendefs.h"
#include "datetypes.h"
#include "tokenizer.h"
#include "exceptions.h"

using namespace std;

void Tokenizer::tokenizeString(string s) {
    this->pos = 0;
    this->repr = s;
}

token Tokenizer::nextToken(string & tokRepr) {
    token t = nextToken2(tokRepr);
    //    cout <<t<<endl;
    return t;
}

token Tokenizer::nextToken2(string & tokRepr) {
    this->skipWhitespaces();
    char ch = this->repr[this->pos];

    //Unarni minus:
    //    if (ch=='-'){
    //        this->getChar();
    //        tokRepr="-";
    //        tokRepr+=this->readNum();
    //        return INT;
    //    }
    if (isdigit(ch)) { //prvni je cislovka
        string num = this->readNum();

        if (this->repr[this->pos] == '.') { // float:
            //            this->pos++; //za tecku
            this->getChar(); //za tecku
            string frac = this->readNum();
            tokRepr = num + "." + frac;
            return DOUBLE;

        } else { // INTEGER
            tokRepr = num;
            return INT;
        }

    } else if (ch == '(') { //zacatek seznamu
        this->getChar();
        return LIST_BEGIN;

    } else if (ch == ')') { //konec seznamu
        this->getChar();
        return LIST_END;

    } else if (ch == '"') { //zacatek stringu
        tokRepr = this->readText();
        return STRING_BEGIN;

    } else if (!isdigit(ch) && isgraph(ch)) { //symbol
        tokRepr = this->readSymbol();
        return SYMBOL;
    }

    return 0;
}

void Tokenizer::skipWhitespaces() {
    while (this->pos < (int) this->repr.length()) {
        //        int chr;
        int chr = this->getChar();
        if (chr == '#') { //přeskakování komentářů
            do {
                chr = this->getChar();
            } while (chr != -1 && chr != '\n');

            this->ungetChar();
        }

        if (!isspace(chr)) {
            this->ungetChar();
            break;
        }
    }
}

string Tokenizer::readNum() {
    string s;
    char chr;
    while (isdigit(chr = this->getChar())) {
        s.push_back(chr);
    }
    this->ungetChar();
    return s;
}

//! je validni znak pro symbol

bool ischar(int chr) {
    if (!isspace(chr) && chr != '(' && chr != ')') {
        return true;
    } else {
        return false;
    }
}

string Tokenizer::readSymbol() {
    string s;
    char chr;
    while (ischar(chr = this->getChar())) {
        s.push_back(chr);
    }
    //    this->pos--;
    this->ungetChar();
    return s;
}

string Tokenizer::readText() {
    string s;
    this->getChar(); // prvni '"'
    int chr = this->getChar();
    while (chr != '"' && chr != -1) {
        s.push_back(chr);
        chr = this->getChar();
    }
    if (chr == -1) { //neuzavřený řetězec
        SyntaxException e;
        e.desc = "Syntax error: Unexpected EOF inside of string";
        e.line = this->line;
        e.col = this->col;

        throw e;
    }

    return s;
}

int Tokenizer::getChar() {
    if (this->pos >= (int) this->repr.length()) {
        return -1;
    }

    //        cout <<"li: "<<this->line <<endl;
    int chr = this->repr[this->pos];
    this->pos++;

    if (chr == '\n') {
        this->line++;
        this->col = 0;
    } else {
        this->col++;
    }
    return chr;
}

void Tokenizer::ungetChar() {
    this->pos--;
    int chr = this->repr[this->pos];

    if (chr == '\n') {
        this->line--;
        //TODO: vlozit misto 0 pozici konce radku:
        this->col = 0;
    } else {
        this->col--;
    }
}
