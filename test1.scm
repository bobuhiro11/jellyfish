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
      (+ x (g (- x 1))))))

(display (+ (+ (+ 1) 2) (+ 1 2))) ; => 6

(cons (+ 1 0) (cons 2 (cons 3 nil)))    ; => (1 2 3)

(display (- 100 (- (- 10)) (- 10 40)))  ; => 120
(display (+ 100 (+ (+ 10)) (+ 10 40)))  ; => 160

(list (+ 0 1) (- 4 2) (- 6 3))
(quote (2 3))
(display (append (quote ( 2 3)) (quote (4 5))))
(display (<= (+ 2 3) (+ 2 3 (- 0 1)))) ; => #f
(display (+ (+ x 5) x))
