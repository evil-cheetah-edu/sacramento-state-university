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
    ; Recursion Function Definition
    (letrec (
        (length_func (lambda (slice result)
            (if (null? slice)
                result
                (length_func (cdr slice) (+ result 1))
            ))
        )
    )
    
    ; Main Call
    (length_func list 0))
)


; (maxmin `<List>) Implementation
(define (maxmin list)
    (if (null? list)
        ; Throw Exception if `list` is empty 
        (error "Empty list has no max or min")
        
        ; Main Call
        (letrec (
            ; Function Definition
            (maxmin_func (lambda (list current-max current-min)
                (if (null? list)
                    (cons current-max current-min)
                    (
                        let (
                            (head (car list))
                            (tail (cdr list))
                        )
                        (maxmin_func tail 
                            (if (> head current-max) head current-max)
                            (if (< head current-min) head current-min)
                        )
                    )
                ))
            )
        )

        ; Function Call
        (maxmin_func (cdr list) (car list) (car list)))
    )
)