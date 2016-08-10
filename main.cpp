
#include <cstdlib>

#include "repl.h"
#include "enviroment.h"

using namespace std;

int main(int argc, char** argv) {
    
    Enviroment env;

    proccessFile(env, "stdlib/init.lisp");
    
    if (argc == 1) {
        Repl REPL;
        REPL.run(env);
    } else {
        for (int i = 0; i < (argc - 1); i++) {
            proccessFile(env, argv[i + 1]);
        }
    }

    return (EXIT_SUCCESS);
}
