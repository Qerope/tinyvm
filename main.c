#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

static void usage(void)
{
    fprintf(stderr, "usage: tinyvm [-d] prog.asm\n");
    exit(2);
}

static char *slurp(const char *path, size_t *len)
{
    FILE *f = fopen(path, "rb");
    long size;
    char *buf;

    if (!f) {
        perror(path);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    buf = malloc(size + 1);
    if (fread(buf, 1, size, f) != (size_t)size) {
        perror(path);
        exit(1);
    }
    buf[size] = 0;
    fclose(f);
    *len = size;
    return buf;
}

static struct vm vm;

int main(int argc, char **argv)
{
    size_t len;
    uint8_t *code;
    char *src;
    int dump = 0;
    int i = 1;

    for (; i < argc && argv[i][0] == '-'; i++) {
        if (strcmp(argv[i], "-d") == 0)
            dump = 1;
        else
            usage();
    }
    if (i >= argc)
        usage();

    src = slurp(argv[i], &len);
    if (assemble(src, &code, &len) != 0)
        return 1;

    if (dump) {
        disasm(code, len);
        return 0;
    }

    vm.code = code;
    vm.len = len;
    vm_run(&vm);
    return 0;
}
