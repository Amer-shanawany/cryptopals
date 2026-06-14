#include "bytes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bytes_t* bytes_new(size_t length) {
    if (!length)
        return NULL;

    bytes_t* new = (bytes_t*)malloc(sizeof(bytes_t*));
    if (!new)
        return NULL;

    new->data = (unsigned char*)malloc(length);
    if (!new->data) {
        free(new);
        return NULL;
    }

    memset((void*)new->data, 0x0, length);

    new->length = length;
    return new;
}

bytes_t* bytes_new_from_string(char* string, size_t length) {
    if (!string)
        return NULL;

    if (!length)
        return NULL;

    bytes_t* new = bytes_new(length);
    if (!new)
        return NULL;

    memcpy(new->data, string, length);

    return new;
}

void bytes_free(bytes_t** bytes) {
    if (*bytes) {
        if ((*bytes)->data) {
            free((*bytes)->data);
            (*bytes)->data   = NULL;
            (*bytes)->length = 0;
        }
        free(*bytes);
        *bytes = NULL;
    }
}

bytes_t* hex_string_to_bytes(char* input) {
    if (!input)
        return NULL;

    size_t length = strlen(input);
    if (length % 2) {
        return NULL;
    }
    length /= 2;

    bytes_t* output = (bytes_t*)malloc(sizeof(bytes_t*));
    if (!output)
        return NULL;

    output->data = (unsigned char*)malloc(length);
    if (!output->data) {
        return NULL;
    }
    output->length = length;

    memset(output->data, 0, length);

    for (unsigned int i = 0; i < length; i++) {
        int count = sscanf(input + (2 * i), "%2hhx", output->data + i);
        if (count != 1) {
            bytes_free(&output);
            return NULL;
        }
    }

    return output;
}

char* bytes_to_hex_string(bytes_t* input) {
    if (!input || !input->data || !input->length)
        return NULL;

    char* output = (char*)malloc(input->length * 2 + 1);
    if (!output)
        return NULL;

    for (unsigned int i = 0; i < input->length; i++) {
        sprintf(&output[i * 2], "%02hhX", input->data[i]);
    }

    output[input->length * 2 + 1] = '\0';
    return output;
}

char* bytes_to_string(bytes_t* input) {
    if (!input || !input->data || !input->length)
        return NULL;

    char* result = malloc(input->length + 1);
    if (!result)
        return NULL;

    memcpy(result, input->data, input->length);
    result[input->length] = '\0';

    return result;
}

bytes_t* bytes_xor(bytes_t* first, bytes_t* second) {
    if (!first || !second)
        return NULL;

    if (!first->data || !second->data)
        return NULL;

    if (!first->length || !second->length)
        return NULL;

    if (first->length != second->length)
        return NULL;

    if (first->length % 2)
        return NULL;

    bytes_t* output = bytes_new(first->length);
    if (!output)
        return NULL;

    for (unsigned int i = 0; i < first->length; i++)
        output->data[i] = first->data[i] ^ second->data[i];

    return output;
}
