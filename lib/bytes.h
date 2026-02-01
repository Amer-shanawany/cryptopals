#ifndef _BYTES_H_
#define _BYTES_H_

#include <stddef.h>

typedef struct {
    size_t length;
    unsigned char* data;
} bytes_t;

bytes_t* bytes_new(size_t length);
bytes_t* bytes_new_from_string(char* string, size_t length);

void bytes_free(bytes_t** bytes);

bytes_t* hex_string_to_bytes(char* input);
char* bytes_to_hex_string(bytes_t* bytes);

bytes_t* bytes_xor(bytes_t* first, bytes_t* second);

#endif // _BYTES_H_