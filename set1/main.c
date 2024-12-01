#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const char lut[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '+', '/', '=' };

void char_to_base64(char* input)
{
    unsigned int in_len = strlen(input);

    // check if the input needs padding
    bool needs_padding = (bool)(in_len % 3);

    int out_size;
    int padding_len = 0;
    if (needs_padding) {
        out_size = (in_len / 3 + 1) * 4;
    } else {
        out_size = in_len / 3 * 4;
    }

    char output[out_size + 1];
    memset(output, '=', out_size);

    for (int len = 0; len < in_len; len = len + 3) {
        char* c = input + len;
        int out_i = len * 8 / 6;

        int first = *c >> 2;
        output[out_i] = lut[first];

        int second = (*c & 0x03) << 4;
        output[out_i + 1] = lut[second];
        if (len + 1 < in_len) {
            c++;
            int second_b = *c >> 4;
            second |= second_b;
        } else
            break;
        output[out_i + 1] = lut[second];

        int third = (*c & 0x0F) << 2;
        output[out_i + 2] = lut[third];
        if (len + 2 < in_len) {
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

    printf("%s\n", output);
}

void hex_to_ascii(char* input, char* output)
{
    for (int i = 0; i < strlen(input); i = i + 2) {
        char* c = input + i;
        unsigned int result, left, right;

        for (int x = 0; x < 2; x++) {
            unsigned int temp;
            c = c + x;
            if (*c >= '0' && *c <= '9') {
                temp = *c - '0';
            } else if (*c >= 'a' && *c <= 'f') {
                temp = *c - 0x57;
            } else if (*c >= 'A' && *c <= 'F') {
                temp = *c - 0x37;
            } else {
                printf("hex_to_ascii: bad input\n");
                return;
            }
            if (!x) {
                left = temp;
            } else {
                right = temp;
            }
        }
        result = left << 4;
        result |= (right & 0x0F);
        output[i / 2] = result;
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: %s hex\n", argv[0]);
        return 1;
    }

    int in_len = strlen(argv[1]);
    if (!in_len) {
        printf("error word is empty\n");
        return 1;
    }
    if (in_len % 2) {
        printf("odd input\n");
        return 1;
    }
    char ascii[in_len / 2 + 1];
    ascii[in_len / 2 + 1] = '\0';
    hex_to_ascii(argv[1], ascii);
    printf("%s\n", ascii);
    char_to_base64(ascii);
    return 0;
}
