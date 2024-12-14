#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set1.h"

const char lut[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '+', '/', '=' };

bytes_t* new_bytes(size_t length)
{
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

    new->length = length;
    return new;
}

void free_bytes(bytes_t* bytes)
{
    if (bytes) {
        if (bytes->data) {
            free(bytes->data);
            bytes->data = NULL;
        }
        free(bytes);
        bytes = NULL;
    }
}

char* bytes_to_base64(bytes_t* input)
{
    if (!input || !input->data || !input->length)
        return NULL;

    // check if the input needs padding
    bool needs_padding = (bool)(input->length % 3);

    int out_size;
    int padding_len = 0;
    if (needs_padding) {
        out_size = (input->length / 3 + 1) * 4;
    } else {
        out_size = input->length / 3 * 4;
    }

    char* output = malloc(out_size + 1);
    if (!output)
        return NULL;

    memset(output, '=', out_size);

    for (int len = 0; len < input->length; len = len + 3) {
        char* c = input->data + len;
        int out_i = len * 8 / 6;

        int first = *c >> 2;
        output[out_i] = lut[first];

        int second = (*c & 0x03) << 4;
        output[out_i + 1] = lut[second];

        if (len + 1 < input->length) {
            c++;
            int second_b = *c >> 4;
            second |= second_b;
        } else
            break;
        output[out_i + 1] = lut[second];

        int third = (*c & 0x0F) << 2;
        output[out_i + 2] = lut[third];
        if (len + 2 < input->length) {
            c++;
            int third_b = *c >> 6;
            third |= third_b;
        } else
            break;
        output[out_i + 2] = lut[third];

        int fourth = (*c & 0x3F);
        output[out_i + 3] = lut[fourth];
    }

    output[out_size + 1] = '\0';

    return output;
}

bytes_t* hex_string_to_bytes(char* input)
{
    if (!input)
        return NULL;

    size_t length = strlen(input);
    if (length % 2) {
        fprintf(stderr, "hex_string_to_bytes: odd number of characters\n");
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
        int count = sscanf(input + 2 * i, "%2hhx", output->data + i);
        if (count != 1) {
            fprintf(stderr, "hex_string_to_bytes: string contains non hex\n");
            free_bytes(output);
            return NULL;
        }
    }

    return output;
}

char* bytes_to_hex_string(bytes_t* input)
{
    if (!input || !input->data || !input->length)
        return NULL;

    char* output = (char*)malloc(input->length * 2 + 1);
    if (!output)
        return NULL;

    for (unsigned int i = 0; i < input->length; i++) {
        sprintf(&output[i * 2], "%2hhX", input->data[i]);
    }

    output[input->length * 2 + 1] = '\0';
    return output;
}

bytes_t* xor_bytes(bytes_t* first, bytes_t* second)
{
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

    bytes_t* output = new_bytes(first->length);
    if (!output)
        return NULL;

    for (unsigned int i = 0; i < first->length; i++)
        output->data[i] = first->data[i] ^ second->data[i];

    return output;
}

unsigned int rate_etaoin_shrdlu(char* input)
{
    if (!input)
        return 0;

    size_t length = strlen(input);
    if (!length)
        return 0;

    // check if this is a readable string
    bool valid = true;
    for (int i = 0; i < length; i++) {
        if (input[i] < 32 || input[i] > 126)
            break;
    }

    if (!valid)
        return 0;

    unsigned int rating = 0;
    for (int i = 0; i < length; i++) {
        switch (input[i]) {
        case 'E':
        case 'e':
            rating += 12;
            break;
        case 'A':
        case 'a':
        case 'I':
        case 'i':
            rating += 9;
            break;
        case 'O':
        case 'o':
            rating += 8;
            break;
        case 'N':
        case 'n':
        case 'R':
        case 'r':
        case 'T':
        case 't':
            rating += 6;
            break;
        case 'L':
        case 'l':
        case 'S':
        case 's':
        case 'U':
        case 'u':
        case 'D':
        case 'd':
            rating += 4;
            break;
        case 'g':
            rating += 3;
            break;
        case 'b':
        case 'c':
        case 'm':
        case 'p':
        case 'f':
        case 'h':
        case 'v':
        case 'w':
        case 'y':
        case ' ':
            rating += 2;
            break;
        case 'k':
        case 'j':
        case 'x':
        case 'q':
        case 'z':
            rating += 1;
            break;
        }
    }

    return rating;
}
