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

(display ((lambda (z) (+ z 1)) 4))
(display ((lambda (x) (+ x 1)) ((lambda () 1))))

(define func1 (lambda () 2))
(display (func1))
(define func2 (lambda () (+ 2 3)))
(display (func2))
(display
  (g 10))

(define plus2
  (lambda (x)
    (+ x 2)))
(define plus4
  (lambda (x)
    (+ x 4)))
(define sample
  (lambda (f x)
    (f x)))
(display (sample plus4 10))
(display (sample plus2 10))
