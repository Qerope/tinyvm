; fill a heap buffer with 1..5, then sum it
    push 5
    alloc
    store 0         ; buf
    push 0
    store 1         ; i
fill:
    load 1
    push 5
    lt
    jz sum_init
    load 0
    load 1
    add             ; addr
    load 1
    push 1
    add             ; i+1
    storeh
    load 1
    push 1
    add
    store 1
    jmp fill
sum_init:
    push 0
    store 2         ; total
    push 0
    store 1
sum:
    load 1
    push 5
    lt
    jz done
    load 0
    load 1
    add
    loadh
    load 2
    add
    store 2
    load 1
    push 1
    add
    store 1
    jmp sum
done:
    load 2
    print
    halt
