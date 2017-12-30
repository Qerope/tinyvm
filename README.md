# tinyvm

A tiny stack-based virtual machine. 32-bit integer stack, separate return
stack, 16 global slots.
Programs are written in a simple assembly language and assembled on load:

    make
    ./tinyvm examples/fib.asm
