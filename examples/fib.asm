; first ten fibonacci numbers
    push 0
    store 0         ; a
    push 1
    store 1         ; b
    push 10
    store 2         ; counter
loop:
    load 2
    jz done
    load 0
    print
    load 0
    load 1
    add             ; a+b
    load 1
    store 0         ; a = b
    store 1         ; b = a+b
    load 2
    push 1
    sub
    store 2
    jmp loop
done:
    halt
