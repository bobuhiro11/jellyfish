;
; 階乗
;
(define f 
  (lambda (x)
    (if (<= x 1) 
      1 
      (* x (f (- x 1))))))

(f 1)
(f 2)
(f 3)
(f 4)
(f (- 9 4))

;
; リスト操作
;
(cons (cons 1 2) (cons 3 4))
(list (+ 0 1) (/ 4 2) (- 6 3))

(define  tail
  (lambda (x)
    (if (nil? (cdr x)) (car x)
      (tail (cdr x)))))
(tail (quote (3 4 5)))

;
; 最大値
;
(define max 
  (lambda (x)
    (if (nil? (cdr x))
      (car x) 
      (if (> (car x) (max (cdr x))) 
        (car x) 
        (max (cdr x))))))
(max (quote (4 2 3 1)))

;
; 最大公約数
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

(gcd2 30 9)
