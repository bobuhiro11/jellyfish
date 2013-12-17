(+ 1)
(display 1)
(newline)

(display (if #t
           1
           2))
(newline)

(display ((lambda (x) (+ x 10)) 100))
(newline)

(define x 5)
(define y x)

(define g
  (lambda (x)
    (if (<= x 1)
      1
      (+ x (g (- x 1))))))

(display (cons 1 2))
(newline)

(list (+ 0 1) (+ 4 2) (+ 6 3))
(quote (2 3))
(append (quote (2 3)) (quote (4 5)))

(atom? 1)
(atom? (quote (2 3)))
(+ 1 5)

(define f (lambda (x) x))
(display (f 100))
(newline)
(display (f 200))
(newline)
(display (f 300))
(newline)

(display (- 300 10 20 30))
(newline)
(display (<= 5 6))
(newline)

(define keep
  (lambda (x)
    x))

(define plus2
  (lambda (x)
    (+ x 2)))

(newline)

(display (keep 4))
(newline)
;(display (plus2 4))
;(newline)
