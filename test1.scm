(+ 1)
(display 1)

(display (if #t
           1
           2))
(lambda (x) (+ x 1))
(define x 5)
(define y x)
(define f (lambda (x) (+ x 1)))

(define g
  (lambda (x)
    (if (<= x 1)
      1
      (* x (f (- x 1))))))

(display (cons 1 2))
(list (+ 0 1) (+ 4 2) (+ 6 3))
(quote (2 3))
(append (quote (2 3)) (quote (4 5)))

(atom? 1)
(atom? (quote (2 3)))
