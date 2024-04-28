; ((inc_n increment) number) Implementation
(define inc_n
    (lambda (increment)
        (lambda (number)
            (+ number increment)
        )
    )
)


; (len <List>) Implementation
(define (len list)
    ; Recursion Function Definition
    (letrec (
        (length-func (lambda (slice result)
            (if (null? slice)
                result
                (length-func (cdr slice) (+ result 1))
            ))
        )
    )
    
    ; Main Call
    (length-func list 0))
)


; (maxmin <List>) Implementation
(define (maxmin list)
    (if (null? list)
        ; Throw Exception if `list` is empty 
        (error "Empty list has no max or min")
        
        ; Main Call
        (letrec (
            ; Function Definition
            (maxmin-func (lambda (list current-max current-min)
                (if (null? list)
                    (cons current-max current-min)
                    (
                        let (
                            (head (car list))
                            (tail (cdr list))
                        )
                        (maxmin-func tail 
                            (if (> head current-max) head current-max)
                            (if (< head current-min) head current-min)
                        )
                    )
                ))
            )
        )

        ; Function Call
        (maxmin-func (cdr list) (car list) (car list)))
    )
)


; (mem item <List>) Implementation
(define (mem item list)
    (cond
        ((null? list) #f)
        ((equal? item (car list)) #t)
        (else (mem item (cdr list)))
    )
)


; (ins item <List>) Implementation
(define (ins item list)
    (if (mem item list)
        list
        (cons item list)
    )
)


; (numT (bool lambda) <List>) Implementation
(define (numT bool-lambda list)
    ; Function Definition
    (letrec (
        (count-true (lambda (list result)
            (if (null? list)
                result
                (count-true
                    (cdr list) 
                    (if (bool-lambda (car list))
                        (+ result 1)
                        result
                    )
                )
            ))
        )
    )

    ; Main Call
    (count-true list 0))
)


; (moreT bool-lambda <Left-List> <Right-List>) Implementation
(define (moreT bool-lambda left-list right-list)
    (
        let (
            (left-count  (numT bool-lambda left-list))
            (right-count (numT bool-lambda right-list))
        )
        
        (cond
            ((> left-count right-count) 1) ; Return `1` if First  List Result is Bigger 
            ((< left-count right-count) 2) ; Return `2` if Second List Result is Bigger
            (else 0)
        )
    )
)