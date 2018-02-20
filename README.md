# tinyvm

A tiny stack-based virtual machine. 32-bit integer stack, separate return
stack, 16 global slots.
Programs are written in a simple assembly language and assembled on load:

    make
    ./tinyvm examples/fib.asm
Flags:

    -d        disassemble instead of running
    -t        trace every instruction to stderr
    -o FILE   assemble to bytecode and write it to FILE
