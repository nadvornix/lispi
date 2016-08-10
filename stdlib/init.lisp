
# STDLIB - Initialization

# defun - Macro for defining functions
# - it makes anonymouse function and then assign name to it.
# - argument body has to contain just one statement. More complex functions can be contained in  
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

# import - function for importing and evaluating another source code
#
(defun import (filename)
    (evall 
        (progn (parse (readfile filename)))
        (exportall)))



(import "stdlib/io.lisp")
(import "stdlib/math.lisp")

