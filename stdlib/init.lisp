
# STDLIB - Initialization

# defun - Makro pro pohodlné vytváření funkcí
# - udělá anonymní funkci kterou následně pojmenuje.
# - parametr body musí obsahovat jen jeden příkaz. Složitější funkce se dávají 
#   do (evall A B C)
# example:
# (defun helloworld ()
#   (print "hello world"))

(defmacro defun (name params body)
    (backquote (set 
              (comma name) 
              (comma (backquote (lambda
                                 (comma params)
                                 (comma body)))))))

# import - funkce pro vložení a interpretaci jiného zdrojáku
#
(defun import (filename)
    (evall 
        (progn (parse (readfile filename)))
        (exportall)))



(import "stdlib/io.lisp")
(import "stdlib/math.lisp")

