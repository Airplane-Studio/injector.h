#ifndef _INJECTOR_H
#define _INJECTOR_H

// inject <len> bytes of <data> into file <filename>. always assumes that file <filename> does exist.
void inject(char *filename, char *data, int len);
// read the injected data into file <filename>. if anything failed or no data injected, return NULL.
char *get_inject_content(char *filename);

#ifdef _INJECTOR_IMPL

#define INJECTION_MAGIC 0x494A4354

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

typedef struct {
    uint32_t magic; // IJCT 0x494A4354
    uint32_t len;
} injection_magic_t;

static void write_int(FILE *fp, uint32_t number) {
    fputc(number >> 24, fp);
    fputc(number >> 16 & 0xff, fp);
    fputc(number >> 8 & 0xff, fp);
    fputc(number & 0xff, fp);
}

static uint32_t read_int(FILE *fp) {
    uint32_t hi_hi = fgetc(fp);
    uint32_t hi_lo = fgetc(fp);
    uint32_t lo_hi = fgetc(fp);
    uint32_t lo_lo = fgetc(fp);
    return (hi_hi << 24) | (hi_lo << 16) | (lo_hi << 8) | lo_lo;
}

static void write_mag(FILE *fp, injection_magic_t mag) {
    write_int(fp, mag.magic);
    write_int(fp, mag.len);
}

static void read_mag(FILE *fp, injection_magic_t *mag) {
    mag->magic = read_int(fp);
    mag->len = read_int(fp);
}

void inject(char *filename, char *data, int len) {
    FILE *fp = fopen(filename, "ab+");
    printf("injecting data into %s\n", filename);
    if (!fp) return;
    fseek(fp, 0, SEEK_END);
    fwrite(data, len, sizeof(char), fp);
    fflush(fp);
    injection_magic_t mag;
    mag.magic = INJECTION_MAGIC;
    mag.len = len;
    write_mag(fp, mag);
    fflush(fp);
    fclose(fp);
}

char *get_inject_content(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    if (size < sizeof(injection_magic_t)) return NULL;
    fseek(fp, -sizeof(injection_magic_t), SEEK_END);
    injection_magic_t mag;
    read_mag(fp, &mag);
    if (mag.magic != INJECTION_MAGIC) return NULL;
    if (size < sizeof(injection_magic_t) + mag.len) return NULL;
    fseek(fp, -sizeof(injection_magic_t) - mag.len, SEEK_END);
    char *buf = (char *) malloc(mag.len + 5);
    memset(buf, 0, mag.len + 5);
    if (!buf) return NULL;
    fread(buf, mag.len, sizeof(char), fp);
    fclose(fp);
    return buf;
}

#endif
#endif