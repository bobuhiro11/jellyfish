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

;
; ソート
;

; (large 5 (quote (2 5 9))) => (5 9)
(define large
  (lambda (x y)
    (if (nil? y)
      nil
      (if (>= (car y) x)
        (cons (car y) (large x (cdr y)))
        (large x (cdr y))))))
(large 5 (quote (2 5 9)))

; (small 5 (quote (2 5 9))) => (2)
(define small
  (lambda (x y)
    (if (nil? y)
      nil
      (if (< (car y) x)
        (cons (car y) (small x (cdr y)))
        (small x (cdr y))))))
(small 5 (quote (2 5 9)))

; (quicksort (quote (3 4 6 4 ))) => (3 4 6 4)
; (define quicksort
;   (lambda (x)
;     (if (nil? x)
;       nil
;       (append
;         (quicksort (large (car x) x))
;         (quicksort (small (car x) x))))))
