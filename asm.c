#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

#define MAX_CODE 65536

static uint8_t out[MAX_CODE];
static size_t n;

static int find_op(const char *name)
{
    for (int i = 0; i < OP_COUNT; i++)
        if (strcmp(opinfo[i].name, name) == 0)
            return i;
    return -1;
}

static void emit32(int32_t v)
{
    memcpy(out + n, &v, 4);
    n += 4;
}

int assemble(const char *src, uint8_t **code, size_t *len)
{
    char *buf = strdup(src);
    int lineno = 0;

    n = 0;
    for (char *line = strtok(buf, "\n"); line; line = strtok(NULL, "\n")) {
        char word[32], arg[32];
        char *c = strchr(line, ';');
        int nargs;

        lineno++;
        if (c)
            *c = 0;
        nargs = sscanf(line, "%31s %31s", word, arg);
        if (nargs < 1)
            continue;
        int op = find_op(word);
        if (op < 0) {
            fprintf(stderr, "line %d: unknown op %s\n", lineno, word);
            return -1;
        }
        out[n++] = op;
        if (opinfo[op].has_imm) {
            if (nargs < 2) {
                fprintf(stderr, "line %d: %s needs an operand\n", lineno, word);
                return -1;
            }
            emit32(strtol(arg, NULL, 0));
        }
    }
    free(buf);
    *code = out;
    *len = n;
    return 0;
}
