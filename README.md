# lispi - My lisp dialect

Simple programming language I had made during first year in the college.

- core of language is working.
- it has macros, REPL, reasonable error reporting (see errors.lisp)
- Documentation is in html/
- It has around 2.5k lines of C++ code.
- utils/mylisp.lang is syntax highlighting for lispi for gedit (put symlink to /usr/share/gtksourceview-2.0/language-specs/)
- nice picture of inheritance of classes: html/inherits.html
- Features that are missing and would be fun to add sometime: tail-recursion, garbage collection

- usage: `./lisp program.lisp`

# Example source code.


```
(printnl "Example source code (test.lisp):")

(and True (== 1 0) (print "nothing will be printed"))

# defun - Macro for defining functions
# - it makes anonymous function and then assign name to it.
# - argument body has to have just one statement. More complex functions can be contained in  
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

# Function for computing factorial, using classic recurrent formula.
# example: (fact 5)

(defun fact (x)
    (if (== x 0)
        1
        (* x (fact (- x 1)))))

(printnl (fact 5))
```

See html/stdlibc_8h.html and stdlib/ for more examples.
