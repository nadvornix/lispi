
#include <string>
#include <cstring>
#include "exceptions.h"

string strformat(const char* format, ...) {

    int len = (strlen(format) + 256)*2; // velikost bufferu pro výsledný desc. Může přetéct

    char * buffer = new char [len];

    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);

    string descS = string(buffer);

    va_end(args);
    delete [] buffer;

    return descS;
}




