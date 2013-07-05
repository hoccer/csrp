
#ifndef SRP_UTILS_H
#define SRP_UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <alloca.h>

#include <srp.h>

int ngtype_from_string(const char *str, SRP_NGType *ngtype);
int halgo_from_string(const char *str, SRP_HashAlgorithm *algo);

void bail(const char *message);

uint8_t hex2nibble(char c);
char nibble2hex(uint8_t n);

void hex2bin(const char *src, size_t srclen, uint8_t *dst, size_t *dstlen);
void bin2hex(const uint8_t *src, size_t srclen, char *dst, size_t *dstlen);

size_t hexask(const char *label, uint8_t *dst, size_t len);
size_t hexread(char *src, uint8_t *dst, size_t len);
void hexdump(const char *label, const uint8_t *src, size_t size);

#endif /* !SRP_UTILS_H */
