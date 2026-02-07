#ifndef _BASE64_H_
#define _BASE64_H_

#include <bytes.h>

char* bytes_to_base64(bytes_t* input);
bytes_t* base64_to_bytes(char *input);

#endif//_BASE64_H_