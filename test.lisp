
(printnl "ukázkový zdroják (test.lisp):")

(and True (== 1 0) (print "nic se nevypíše"))

# Funkce pro výpočet faktorialu klasickým rekurentním předpisem.
# pozn: Lispovský typ LInt pouziva pro vnitrni reprezentaci obyčejný int takže
# výsledek může snadno přetéct.
#example: (fact 5)

(defun fact (x)
    (if (== x 0)
        1
        (* x (fact (- x 1)))))

(printnl (fact 5))

