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

;
; 最大値
;
; (define max 
;   (lambda (x)
;     (if (nil? x) 0 (max (cdr x)))))
;
; 最大公約数
;
; (define gcd
;   (lambda (x y) 
;     (if (> x y) (gcd2 x y)
;                 (gcd2 y x))))
; 
; (define gcd2 (lambda (x y)
;     (if (= y 0) x
;                 (gcd2 y (modulo x y)))))
; 
; (gcd 30 9)
