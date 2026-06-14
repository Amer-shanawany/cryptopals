#ifndef _AES_H_
#define _AES_H_

#include <bytes.h>

#define STATE_SIZE 16
#define STATE_LENGTH 4
#define STATE_ROWS STATE_LENGTH
#define STATE_COLS STATE_LENGTH

enum aes {
    AES_128 = 128,
    AES_192 = 192,
    AES_256 = 256,
};

enum aes_key_size {
    AES_128_KEY_SIZE = 4,
    AES_192_KEY_SIZE = 6,
    AES_256_KEY_SIZE = 8,
};

enum aes_key_rounds {
    AES_128_KEY_ROUNDS = 10,
    AES_192_KEY_ROUNDS = 12,
    AES_256_KEY_ROUNDS = 14,
};

struct aes_context {
    enum aes aes;
    enum aes_key_size key_size;
    enum aes_key_rounds key_rounds;
    bytes_t* state;
    bytes_t* key_schedule;
    bytes_t* key;
};

struct aes_context* aes_init_context(enum aes aes);
void aes_free_context(struct aes_context** ctx);
int aes_key_expansion(struct aes_context* ctx);

void aes_shiftrows(unsigned char* state);
void aes_inv_shiftrows(unsigned char* state);
void aes_mix_columns(unsigned char* state);
void aes_inv_mix_columns(unsigned char* state);
void aes_add_round_key(unsigned char* state, unsigned char* round_key);
void aes_subword(unsigned char* word);
bytes_t* aes_encrypt(char* input, char* key, enum aes aes);
bytes_t* aes_decrypt(char* input, char* key, enum aes aes);

#endif //_AES_H_
