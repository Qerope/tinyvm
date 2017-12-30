#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

static void usage(void)
{
    fprintf(stderr, "usage: tinyvm prog.asm\n");
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
    int i = 1;

    if (i >= argc)
        usage();

    src = slurp(argv[i], &len);
    if (assemble(src, &code, &len) != 0)
        return 1;

    vm.code = code;
    vm.len = len;
    vm_run(&vm);
    return 0;
}
