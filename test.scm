;
; $B%5%s%W%k%3!<%I(B
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
