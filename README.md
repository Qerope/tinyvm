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
## Instruction set

    push N     push immediate
    add sub mul div lt eq
    dup swap drop
    jmp L      jz L (pops, jumps if zero)
    call L     ret
    load N     store N    (global slots)
    alloc      pops size, pushes heap address
    loadh      storeh     (storeh pops value, then address)
    putc       print top of stack as a character
    print      print top of stack as a number
    halt

Labels are `name:` on their own line, comments start with `;`.
Division by zero, stack overflow/underflow and bad opcodes stop the VM with
an error and exit status 1.
