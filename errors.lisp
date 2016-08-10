## Some broken source codes 


## Undefined variable
#(printnl foo)


## error in nested function (longer traceback):
(defun foo () (bar))
(defun bar () (baz))
(defun baz () (lat))

(foo)



# Syntax error:
#(print "


