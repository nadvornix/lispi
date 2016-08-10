
# I/O LIBRARY

# newline
(set NL "
")

# function printendl prints new line
(defun printendl ()
    (print NL))

# function printnl - prints arguments followed by \n 
(defun printnl (what)
    (evall (print what)
           (printendl)))


