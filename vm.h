#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stddef.h>

#define STACK_MAX 256
#define RSTACK_MAX 64

enum {
    OP_HALT,
    OP_PUSH,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_PRINT,
    OP_JMP,
    OP_JZ,
    OP_CALL,
    OP_RET,
};

struct vm {
    const uint8_t *code;
    size_t len;
    uint32_t pc;
    int32_t stack[STACK_MAX];
    int sp;
    uint32_t rstack[RSTACK_MAX];
    int rsp;
};

void vm_run(struct vm *vm);

#endif
