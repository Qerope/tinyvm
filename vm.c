#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

const struct opinfo opinfo[OP_COUNT] = {
    [OP_HALT] = {"halt", 0},
    [OP_PUSH] = {"push", 1},
    [OP_ADD] = {"add", 0},
    [OP_SUB] = {"sub", 0},
    [OP_MUL] = {"mul", 0},
    [OP_DIV] = {"div", 0},
    [OP_PRINT] = {"print", 0},
    [OP_JMP] = {"jmp", 1},
    [OP_JZ] = {"jz", 1},
    [OP_CALL] = {"call", 1},
    [OP_RET] = {"ret", 0},
    [OP_LOAD] = {"load", 1},
    [OP_STORE] = {"store", 1},
};

static void die(struct vm *vm, const char *msg)
{
    fprintf(stderr, "vm error at %04x: %s\n", vm->pc, msg);
    exit(1);
}

static void push(struct vm *vm, int32_t v)
{
    vm->stack[vm->sp++] = v;
}

static int32_t pop(struct vm *vm)
{
    if (vm->sp == 0)
        die(vm, "stack underflow");
    return vm->stack[--vm->sp];
}

static int32_t fetch32(struct vm *vm)
{
    int32_t v;

    memcpy(&v, vm->code + vm->pc, 4);
    vm->pc += 4;
    return v;
}

void vm_run(struct vm *vm)
{
    for (;;) {
        if (vm->pc >= vm->len)
            die(vm, "pc out of range");
        int op = vm->code[vm->pc++];

        switch (op) {
        case OP_HALT:
            return;
        case OP_PUSH:
            push(vm, fetch32(vm));
            break;
        case OP_ADD: {
            int32_t b = pop(vm), a = pop(vm);
            push(vm, a + b);
            break;
        }
        case OP_SUB: {
            int32_t b = pop(vm), a = pop(vm);
            push(vm, a - b);
            break;
        }
        case OP_MUL: {
            int32_t b = pop(vm), a = pop(vm);
            push(vm, a * b);
            break;
        }
        case OP_DIV: {
            int32_t b = pop(vm), a = pop(vm);
            push(vm, a / b);
            break;
        }
        case OP_PRINT:
            printf("%d\n", pop(vm));
            break;
        case OP_JMP:
            vm->pc = fetch32(vm);
            break;
        case OP_JZ: {
            uint32_t target = fetch32(vm);
            if (pop(vm) == 0)
                vm->pc = target;
            break;
        }
        case OP_CALL: {
            uint32_t target = fetch32(vm);
            vm->rstack[vm->rsp++] = vm->pc;
            vm->pc = target;
            break;
        }
        case OP_RET:
            if (vm->rsp == 0)
                die(vm, "return stack underflow");
            vm->pc = vm->rstack[--vm->rsp];
            break;
        case OP_LOAD: {
            int32_t n = fetch32(vm);
            if (n < 0 || n >= GLOBALS)
                die(vm, "bad global slot");
            push(vm, vm->globals[n]);
            break;
        }
        case OP_STORE: {
            int32_t n = fetch32(vm);
            if (n < 0 || n >= GLOBALS)
                die(vm, "bad global slot");
            vm->globals[n] = pop(vm);
            break;
        }
        default:
            die(vm, "bad opcode");
        }
    }
}
