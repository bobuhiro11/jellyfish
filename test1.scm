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

(display (cons (+ 1 0) (cons 2 (cons 3 nil))))    ; => (1 2 3)

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

(begin
  (display (plus2 -1))
  (newline)
  (display 2)
  (newline)
  (display 3)
  (newline)
  (display 4)
  (newline))

(display (atom? 1))
(display (< 1 2))
(display (<= 1 1))
(display (>= 1 1))
(display (> 1 1))
(display (nil? nil))
(display (nil? 1))

(display (car (cdr (quote (1 2 3)))))

(define fact
  (lambda (x)
    (if (<= x 1)
      1
      (* x (fact (- x 1))))))
(display (fact 3))
(newline)
(display (fact 4))
(newline)
(display (append (quote (1 2 3)) (quote (4 5 6))))

(define mylist
  (lambda (x y z )
    (list x y z)))
(display (mylist 0 1 2))

(display (mylist 0 1 2))

;(define build_list
;  (lambda (x)
;    (if (= x 0)
;      nil
;      (build_list x (- x 1)))))
;(display (build_list 10))

(define  tail
  (lambda (x)
    (if (null? (cdr x)) (car x)
      (tail (cdr x)))))
(display "tail 3 4 5 =" (tail (quote (3 4 5))))
(newline)

;
; クイックソート
;

; (part >= (quote (2 5 9)) 5) => (5 9)
; (part <  (quote (2 5 9)) 5) => (2)
(define part
  (lambda (c y x)
    (if (null? y)
      nil
      (if (c (car y) x)
        (cons (car y) (part c (cdr y) x))
        (part c (cdr y) x)))))

(display (part >= (quote (2 5 9)) 5))
(newline)
(display (part <  (quote (2 5 9)) 5))
(newline)

(define quicksort
  (lambda (x)
    (if (null? x)
      nil
      ((lambda (pivot)
        (append
          (quicksort (part <  x pivot))
          (cons
            (car x)
            (quicksort (part >  x pivot)))))
       (car x)))))

(define before (quote (20 4 6 2 9 8 10 38 92 88)))
(define after (quicksort before))

(display "before = " before)
(newline)
(display "after  = " after)
(newline)
