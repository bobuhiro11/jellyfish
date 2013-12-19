;
; integration test
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; function definition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;
; factorial
;
(define fact
  (lambda (x)
    (if (<= x 1)
      1
      (* x (fact (- x 1))))))

;
; tail object of list
;
(define  tail
  (lambda (x)
    (if (null? (cdr x)) (car x)
      (tail (cdr x)))))

;
; max value in list
;
(define max
  (lambda (x)
    (if (null? (cdr x))
      (car x)
      (if (> (car x) (max (cdr x)))
        (car x)
        (max (cdr x))))))

;
; gcd
;
(define gcd
  (lambda (x y)
    (if (> x y)
      (gcd2 x y)
      (gcd2 y x))))

(define gcd2
  (lambda (x y)
    (if (= y 0)
      x
      (gcd2 y (modulo x y)))))

;
; quick sort
;
(define (part c y x)
    (if (null? y)
      '()
      (if (c (car y) x)
        (cons (car y) (part c (cdr y) x))
        (part c (cdr y) x))))

(define (quicksort x)
    (if (null? x)
      '()
      ((lambda (pivot)
        (append
          (quicksort (part <  x pivot))
          (cons
            (car x)
            (quicksort (part >  x pivot)))))
       (car x))))
;
; fizz buzz
;
(define check_fizzbuzz
  (lambda (x)
    (if (= (modulo x 15) 0)
      "FizzBuzz"
      (if (= (modulo x 5) 0)
        "Buzz"
        (if (= (modulo x 3) 0)
          "Fizz"
          x)))))

(define fizzbuzz
  (lambda (a b)
    (if (> a b)
      nil
      (begin
        (display (check_fizzbuzz a) " ")
        (fizzbuzz (+ a 1) b)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; function call
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define x '(fizzbuzz 1 50))
(display x " => ") (eval x) (newline)
(define x '(fact 6))
(display x " => " (eval x)) (newline)
(define x '(tail '(3 5 12 213 43)))
(display x " => " (eval x)) (newline)
(define x '(max '(3 5 12 213 43)))
(display x " => " (eval x)) (newline)
(define x '(gcd 15 21))
(display x " => " (eval x)) (newline)
(define x '(quicksort '(32 77 70 21 3 2 73)))
(display x " => " (eval x)) (newline)
