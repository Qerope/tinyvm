#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

static struct vm vm;

int main(int argc, char **argv)
{
    uint8_t prog[] = {
        OP_PUSH, 2, 0, 0, 0,
        OP_PUSH, 3, 0, 0, 0,
        OP_ADD,
        OP_PRINT,
        OP_HALT,
    };

    vm.code = prog;
    vm.len = sizeof prog;
    vm_run(&vm);
    return 0;
}
