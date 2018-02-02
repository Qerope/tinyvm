; recursive factorial, argument and result on the stack
    push 10
    call fact
    print
    halt
fact:
    dup
    push 2
    lt              ; n < 2 ?
    jz recurse
    drop
    push 1
    ret
recurse:
    dup
    push 1
    sub             ; n n-1
    call fact       ; n fact(n-1)
    mul
    ret
