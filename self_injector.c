#define _INJECTOR_IMPL
#include "injector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

char *read_whole_file(char *filename, int *osize) {
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buf = (char *) malloc(size + 5);
    memset(buf, 0, size + 5);
    fread(buf, size, sizeof(char), fp);
    fclose(fp);
    if (osize) *osize = size;
    return buf;
}

int injector_main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <inject file> <output file> [args]\n", argv[0]);
        return 1;
    }
    int inject_size;
    char *inject_data = read_whole_file(argv[1], &inject_size);
    // in order to inject into myself, i must copy myself.
    FILE *fp2 = fopen(argv[2], "wb");
    int myself_size;
    char *myself = read_whole_file(argv[0], &myself_size);
    fwrite(myself, myself_size, sizeof(char), fp2);
    fflush(fp2);
    fclose(fp2);
    free(myself);
    inject(argv[2], inject_data, inject_size);
    chmod(argv[2], 0777);
    free(inject_data);
    return 0;
}

int main(int argc, char *argv[]) {
    bool flag = false;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-r")) {
            flag = true;
            break;
        }
    }
    char *content = get_inject_content(argv[0]);
    if (content && !flag) {
        puts(content);
        free(content);
        return 0;
    }
    return injector_main(argc, argv);
}