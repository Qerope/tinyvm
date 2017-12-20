#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

static void push(struct vm *vm, int32_t v)
{
    vm->stack[vm->sp++] = v;
}

static int32_t pop(struct vm *vm)
{
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
            vm->pc = vm->rstack[--vm->rsp];
            break;
        }
    }
}
