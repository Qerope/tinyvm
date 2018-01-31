#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stddef.h>

#define STACK_MAX 256
#define RSTACK_MAX 64
#define GLOBALS 16

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
    OP_LOAD,
    OP_STORE,
    OP_LT,
    OP_EQ,
    OP_DUP,
    OP_SWAP,
    OP_DROP,
    OP_COUNT
};

struct opinfo {
    const char *name;
    int has_imm;
};

extern const struct opinfo opinfo[OP_COUNT];

struct vm {
    const uint8_t *code;
    size_t len;
    uint32_t pc;
    int32_t stack[STACK_MAX];
    int sp;
    uint32_t rstack[RSTACK_MAX];
    int rsp;
    int32_t globals[GLOBALS];
};

void vm_run(struct vm *vm);
int assemble(const char *src, uint8_t **code, size_t *len);
void disasm(const uint8_t *code, size_t len);

#endif
