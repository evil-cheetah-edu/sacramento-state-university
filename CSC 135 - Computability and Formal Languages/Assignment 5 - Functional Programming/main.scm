; ((inc_n increment) number) Implementation
(define inc_n
    (lambda (increment)
        (lambda (number)
            (+ number increment)
        )
    )
)

; (len `<List>) Implementation
(define (len list)
    (letrec ((length_func (lambda (slice result)
        (if (null? slice)
            result
            (length_func (cdr slice) (+ result 1))
        )
    )))
    (length_func list 0))
)