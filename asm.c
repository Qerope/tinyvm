#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

#define MAX_CODE 65536
#define MAX_LABELS 256

struct label {
    char name[32];
    uint32_t addr;
};

struct fixup {
    uint32_t at;
    char name[32];
};

static struct label labels[MAX_LABELS];
static int nlabels;
static struct fixup fixups[MAX_LABELS * 4];
static int nfixups;

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

static int find_label(const char *name)
{
    for (int i = 0; i < nlabels; i++)
        if (strcmp(labels[i].name, name) == 0)
            return i;
    return -1;
}

int assemble(const char *src, uint8_t **code, size_t *len)
{
    char *buf = strdup(src);
    int lineno = 0;

    n = 0;
    nlabels = nfixups = 0;
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
        if (word[strlen(word) - 1] == ':') {
            word[strlen(word) - 1] = 0;
            if (find_label(word) >= 0) {
                fprintf(stderr, "line %d: duplicate label %s\n", lineno, word);
                return -1;
            }
            snprintf(labels[nlabels].name, sizeof labels[0].name, "%s", word);
            labels[nlabels++].addr = n;
            continue;
        }
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
            if (isdigit((unsigned char)arg[0]) || arg[0] == '-') {
                emit32(strtol(arg, NULL, 0));
            } else {
                snprintf(fixups[nfixups].name, sizeof fixups[0].name, "%s", arg);
                fixups[nfixups++].at = n;
                emit32(0);
            }
        }
    }
    free(buf);

    for (int i = 0; i < nfixups; i++) {
        int l = find_label(fixups[i].name);
        if (l < 0) {
            fprintf(stderr, "undefined label %s\n", fixups[i].name);
            return -1;
        }
        memcpy(out + fixups[i].at, &labels[l].addr, 4);
    }
    *code = out;
    *len = n;
    return 0;
}
