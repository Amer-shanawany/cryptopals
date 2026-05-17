#include "bytes.h"
#include <aes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// the state
#define STATE_SIZE 16
#define STATE_LENGTH 4
#define STATE_ROWS STATE_LENGTH
#define STATE_COLS STATE_LENGTH

// struct aes_state * aes_state_new() {
//     struct aes_state * state = malloc(sizeof(struct aes_state));
//     if (!state) {
//         return NULL;
//     }

//     state->ptr = malloc(STATE_ROWS * sizeof(unsigned char *));
//     if (!state->ptr) {
//         free(state);
//         return  NULL;
//     }


//     for (int i = 0; i < STATE_ROWS; i++) {
//         state->ptr[i] = malloc(STATE_COLS * sizeof(unsigned char *));
//         if (!state->ptr[i]) {
//             free(state->ptr);
//             state->ptr = NULL;
//             free(state);
//             state = NULL;
//             return NULL;
//         }
//     }

//     return state;
// }

// void aes_state_free(struct aes_state ** state) {
//     if(*state) {
//         for (int i = 0; i < STATE_ROWS; i++) {
//             if ((*state)->ptr[i]) { 
//                 free((*state)->ptr[i]);
//                 (*state)->ptr[i] = NULL;
//             }
//         }

//         if ((*state)->ptr) {
//             free((*state)->ptr);
//             (*state)->ptr = NULL;
//         }

//         free(*state);
//         *state = NULL;
//     }
// }

struct aes_key_schedule {
    unsigned char * key;
};

//aes_block_cipher_key_size
enum aes_key_size { // 4 * (NR + 1) words
    AES_128_KEY_SIZE = 4, // => 20
    AES_192_KEY_SIZE = 6, // => 28
    AES_256_KEY_SIZE = 8, // => 36
};

enum aes_key_rounds {
    AES_128_KEY_ROUNDS = 10,
    AES_192_KEY_ROUNDS = 12,
    AES_256_KEY_ROUNDS = 14,
};

struct aes_context {
    // aes_key_size
    // aes_key_rounds
    // ptr to state
    // ptr to key s
};

// Table 5. Round constants
// j Rcon[ j]
const unsigned char round_constant[10][4] = {
    // 1 [01,00,00,00]
    {0x01, 0x0 ,0x0, 0x0},
    // 2 [02,00,00,00]
    {0x02, 0x0 ,0x0, 0x0},
    // 3 [04,00,00,00] 
    {0x04, 0x0 ,0x0, 0x0},
    // 4 [08,00,00,00]
    {0x08, 0x0 ,0x0, 0x0},
    // 5 [10,00,00,00]
    {0x10, 0x0 ,0x0, 0x0},
    // 6 [20,00,00,00]
    {0x20, 0x0 ,0x0, 0x0},
    // 7 [40,00,00,00]
    {0x40, 0x0 ,0x0, 0x0},
    // 8 [80,00,00,00]
    {0x80, 0x0 ,0x0, 0x0},
    // 9 [1b,00,00,00]
    {0x1b, 0x0 ,0x0, 0x0},
    // 10 [36,00,00,00]
    {0x36, 0x0 ,0x0, 0x0},
};


bool aes_key_verify(enum aes_key_size key_size, bytes_t * key) {
    if (!key)
        return false;

    if (!key->data)
        return false;

    if (key->length != key_size)
        return false;

    return true;
}

struct aes_key_schedule * aes_key_schedule_new(enum aes_key_size key_size, bytes_t * key) {
    if (!aes_key_verify(key_size, key))
        return NULL;

    
    return NULL;
}

struct aes_key_word {
    unsigned char * a0;    
    unsigned char * a1;
    unsigned char * a2;
    unsigned char * a3;
};

inline void aes_key_rotword(struct aes_key_word * word) {
    unsigned char * temp = word->a0;
    word->a0 = word->a1; 
    word->a1 = word->a2; 
    word->a2 = word->a3; 
    word->a3 = temp; 
}

// needs ROTWORD and SUBWORD
// aes_key_expansion() {
// 
// }

inline unsigned int get_state_row(unsigned int index) {
    return index % 4;
}

inline unsigned int get_state_column(unsigned int index) {
    return index / 4;
}

inline unsigned int get_output_index(unsigned int row, unsigned int column) {
    return row + (column * 4);
}

void aes_state_free(struct aes_state ** state) {
    if (*state) {
        free(*state);
        *state = NULL;
    }
}

bytes_t * aes_state_input(char * input) {
    if (!input || strlen( input) != STATE_SIZE)
        return NULL;

    return bytes_new_from_string(input, STATE_SIZE);
}

struct aes_state * aes_state_new(bytes_t * input) {
    if (!input || !input->data || input->length != STATE_SIZE)
        return NULL;

    struct aes_state * state = malloc(sizeof(struct aes_state));
    if (!state)
        return NULL;

    memcpy(state->arr, input->data, STATE_SIZE);    

    return state;
}

// Key Expansion

// AES steps
int aes_cipher(char * input, unsigned char * expaned_key, enum aes_key_rounds rounds) {
    // 1 input -> state
    bytes_t *input_bytes = aes_state_input(input);
    if (!input_bytes)
        return -1;

    struct aes_state * state = aes_state_new(input_bytes);
    if (!state)
        return -2;

    // 2 add_round_key

    // 3 for rounds
    //  SUBBYTES
    //  SHIFTROWS
    //  MIXCOLUMNS
    //  ADDROUNDKEY

    //  SUBBYTES
    //  SHIFTROWS
    //  MIXCOLUMNS


    return 0;
}

int aes_invcipher();

// SubKey First (key whitening)

// for round (10, 12 or 14 depends on key length)

// Byte Sub (confusion)
// Shift Row (diffusion)
// Mix Column (diffusion) if not Last round
// Key addition

// SubKey 

unsigned char aes_xTimes(unsigned char b, unsigned char c) {
    unsigned char result = 0;

    while (c) {
        if (c & 0x1)
            result ^= b;
        if (b & 0x80) {
            b = (b << 1) ^ 0x1B;
        } else {
            b <<= 1;
        }
        c >>= 1;
    }

    return result;
}