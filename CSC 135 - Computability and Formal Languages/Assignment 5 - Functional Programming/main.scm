; ((inc_n increment) number) Implementation
(define inc_n
    (lambda (increment)
        (lambda (number)
            (+ number increment)
        )
    )
)
