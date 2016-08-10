
# MATH/LOGIC LIBRARY

# Basic boolean values for use in lisp programms (not for c)
(set True (== 1 1))
(set False (== 1 0))

# Gess or equal 
(defun <= (x y)
    (or (< x y)
        (== x y)))

# Greater or equal
(defun >= (x y)
    (not (< x y)))

# Is argument even?
(defun even? (x)
    (== 0 (% x 2)))

# Is argument odd?
(defun odd? (x)
    (== 1 (% x 2)))


