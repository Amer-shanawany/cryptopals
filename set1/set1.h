#ifndef _SET1_H_
#define _SET1_H_

#include <stddef.h>

typedef struct {
    size_t length;
    unsigned char* data;
} bytes_t;

bytes_t* new_bytes(size_t length);
void free_bytes(bytes_t* bytes);

bytes_t* hex_string_to_bytes(char* input);
char* bytes_to_hex_string(bytes_t* bytes);

char* bytes_to_base64(bytes_t* input);

bytes_t* xor_bytes(bytes_t* fisrt, bytes_t* second);

unsigned int rate_etaoin_shrdlu(char* input);

#endif
