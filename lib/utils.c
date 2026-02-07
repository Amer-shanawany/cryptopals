#include <stdbool.h>
#include <string.h>
#include <utils.h>

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
        case 'G':
        case 'g':
            rating += 3;
            break;
        case 'B':
        case 'b':
        case 'C':
        case 'c':
        case 'M':
        case 'm':
        case 'P':
        case 'p':
        case 'F':
        case 'f':
        case 'H':
        case 'h':
        case 'V':
        case 'v':
        case 'W':
        case 'w':
        case 'Y':
        case 'y':
        case ' ':
            rating += 2;
            break;
        case 'K':
        case 'k':
        case 'J':
        case 'j':
        case 'X':
        case 'x':
        case 'Q':
        case 'q':
        case 'Z':
        case 'z':
            rating += 1;
            break;
		default:
			break;
        }
    }

    return rating;
}

int hamming_distance(bytes_t* first, bytes_t* second)
{
    if (!first || !second)
        return -1;
    if (!first->data || !second->data)
        return -1;
    if (!first->length || !second->length)
        return -1;
    if (first->length != second->length)
        return -1;

    int distance = 0;
    for (int i = 0; i < first->length; i++) {
        char xor = first->data[i] ^ second->data[i];
        while (xor) {
            distance += xor & 0x1;
            xor >>= 1;
        }
    }

    return distance;
}