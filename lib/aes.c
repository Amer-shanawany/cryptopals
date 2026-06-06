#include "bytes.h"
#include <aes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STATE_SIZE 16
#define STATE_LENGTH 4
#define STATE_ROWS STATE_LENGTH
#define STATE_COLS STATE_LENGTH

struct aes_context* aes_init_context(enum aes aes) {
    struct aes_context * ctx = malloc(sizeof(struct aes_context)); 

	if (!ctx)
		return NULL;

	switch (aes) {
		case AES_128:
			ctx->key_size = AES_128_KEY_SIZE;
			ctx->key_rounds = AES_128_KEY_ROUNDS;
		  break;
		case AES_192:
			ctx->key_size = AES_192_KEY_SIZE;
			ctx->key_rounds = AES_192_KEY_ROUNDS;
		  break;
		case AES_256:
			ctx->key_size = AES_256_KEY_SIZE;
			ctx->key_rounds = AES_256_KEY_ROUNDS;
		  break;
		default:
		  return NULL;
	}

	ctx->aes = aes;

	ctx->state = bytes_new(STATE_SIZE);
	if (!ctx->state) {
		free(ctx);
		return NULL;
	}

	ctx->key_schedule = bytes_new((ctx->key_rounds * 4 + 4) * 4);
	if (!ctx->key_schedule) {
		bytes_free(&ctx->state);
		free(ctx);
		return NULL;
	}

	ctx->key = bytes_new(ctx->key_size);
	if (!ctx->key){
		bytes_free(&ctx->key_schedule);
		bytes_free(&ctx->state);
		free(ctx);
		return NULL;
	}

	return ctx;
}

void aes_free_context(struct aes_context ** ctx) {
	if (*ctx) {
		bytes_free(&(*ctx)->key);
		bytes_free(&(*ctx)->key_schedule);
		bytes_free(&(*ctx)->key);
		free(*ctx);
		*ctx = NULL;
	}
}

// Table 4. SBOX()
const unsigned char subtitution_box[16][16] = {
	{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
	{ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
	{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
	{ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
	{ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
	{ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
	{ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
	{ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
	{ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
	{ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
	{ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
	{ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
	{ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
	{ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
	{ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
	{ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};

// Table 5. Round constantsRcon[j]
const unsigned char round_constant[10][4] = {
    {0x01, 0x0 ,0x0, 0x0},
    {0x02, 0x0 ,0x0, 0x0},
    {0x04, 0x0 ,0x0, 0x0},
    {0x08, 0x0 ,0x0, 0x0},
    {0x10, 0x0 ,0x0, 0x0},
    {0x20, 0x0 ,0x0, 0x0},
    {0x40, 0x0 ,0x0, 0x0},
    {0x80, 0x0 ,0x0, 0x0},
    {0x1b, 0x0 ,0x0, 0x0},
    {0x36, 0x0 ,0x0, 0x0},
};

inline unsigned int get_state_row(unsigned int index) {
    return index % 4;
}

inline unsigned int get_state_column(unsigned int index) {
    return index / 4;
}

inline unsigned int get_output_index(unsigned int row, unsigned int column) {
    return row + (column * 4);
}

void aes_subbyte(unsigned char *byte) {
	unsigned char col = *byte >> 4;
	unsigned char row = *byte & 0x0F;
	*byte = subtitution_box[col][row];
}

void aes_subbytes(unsigned char *state) {
    for (int i = 0; i < STATE_SIZE; i++)
        aes_subbyte(state + i);
}

// AES steps
void aes_cipher(char * input, struct aes_context * ctx) {
    // 1 input -> state
    memcpy(ctx->state->data, input, STATE_SIZE);

    // 2 add_round_key
    aes_add_round_key(ctx->state->data, ctx->key_schedule->data);

    for (int round = 1; round < ctx->key_rounds; round++) {
        aes_subbytes(ctx->state->data);
        aes_shiftrows(ctx->state->data);
        aes_mix_columns(ctx->state->data);
        aes_add_round_key(ctx->state->data, (ctx->key_schedule->data + (round * STATE_SIZE)));
    }

    // 3 for rounds
    aes_subbytes(ctx->state->data);
    aes_shiftrows(ctx->state->data);
    aes_add_round_key(ctx->state->data, (ctx->key_schedule->data + ((ctx->key_rounds) * STATE_SIZE)));
}

int aes_invcipher();

// SubKey First (key whitening)

// for round (10, 12 or 14 depends on key length)

// Byte Sub (confusion)
// Shift Row (diffusion)
// Mix Column (diffusion) if not Last round
// Key addition

// SubKey

unsigned char aes_x_times(unsigned char b, unsigned char c) {
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

void aes_rotword(unsigned char * word) {
	unsigned char temp = *word;
	*(word + 0) = *(word + 1);
	*(word + 1) = *(word + 2);
	*(word + 2) = *(word + 3);
	*(word + 3) = temp;
}


void aes_subword(unsigned char *word) {
	for (int i = 0; i < 4; i++) {
		aes_subbyte(word + i);
	}
}

int aes_key_expansion(struct aes_context * ctx) {
	if (!ctx) {
		return 1;
	}

	unsigned char * ks = ctx->key_schedule->data;

	for (int i = 0; i <= ctx->key_size - 1; i++) {
		ks[0 + (i * 4)] = ctx->key->data[0 + (i * 4)];
		ks[1 + (i * 4)] = ctx->key->data[1 + (i * 4)];
		ks[2 + (i * 4)] = ctx->key->data[2 + (i * 4)];
		ks[3 + (i * 4)] = ctx->key->data[3 + (i * 4)];
	}

	int limit = ctx->key_rounds * 4 + 3;
	for (int i = ctx->key_size; i <= limit; i++) {
		unsigned char temp[4];
		temp[0] = ks[ 0 + ((i - 1) * 4)];
		temp[1] = ks[ 1 + ((i - 1) * 4)];
		temp[2] = ks[ 2 + ((i - 1) * 4)];
		temp[3] = ks[ 3 + ((i - 1) * 4)];

		if (!(i % ctx->key_size)) {
			aes_rotword(temp);
			aes_subword(temp);
			temp[0] ^= round_constant[i / ctx->key_size - 1][0];
		} else if (ctx->aes == AES_256 && (i % ctx->key_size == 4)) {
			aes_subword(temp);
		}

		ks[0 + (i * 4)] = ks[0 + ((i - ctx->key_size) * 4)] ^ temp[0];
		ks[1 + (i * 4)] = ks[1 + ((i - ctx->key_size) * 4)] ^ temp[1];
		ks[2 + (i * 4)] = ks[2 + ((i - ctx->key_size) * 4)] ^ temp[2];
		ks[3 + (i * 4)] = ks[3 + ((i - ctx->key_size) * 4)] ^ temp[3];
	}

	return 0;
}

void aes_swap_bytes(unsigned char *a, unsigned char *b) {
	unsigned char temp = *a;
	*a = *b;
	*b = temp;
}

void aes_shiftrows(unsigned char * state) {
	// 2nd row
	aes_swap_bytes(state + 1, state + 5);
	aes_swap_bytes(state + 5, state + 9);
	aes_swap_bytes(state + 9, state + 13);

	// 3rd row
	aes_swap_bytes(state + 2, state + 10);
	aes_swap_bytes(state + 6, state + 14);

	// 4th row
	aes_swap_bytes(state + 3, state + 7);
	aes_swap_bytes(state + 3, state + 11);
	aes_swap_bytes(state + 3, state + 15);
}

void aes_mix_columns(unsigned char * state) {
    for (int i = 0; i < 4; i++) {
        unsigned char * a0 = (state + 0 + (i * 4));
        unsigned char * a1 = (state + 1 + (i * 4));
        unsigned char * a2 = (state + 2 + (i * 4));
        unsigned char * a3 = (state + 3 + (i * 4));

        char r0 = aes_x_times(*a0, 2) ^ aes_x_times(*a1, 3) ^ *a2 ^ *a3;
        char r1 = *a0 ^ aes_x_times(*a1, 2) ^ aes_x_times(*a2, 3) ^ *a3;
        char r2 = *a0 ^ *a1 ^ aes_x_times(*a2, 2) ^ aes_x_times(*a3, 3);
        char r3 = aes_x_times(*a0, 3) ^ *a1 ^ *a2 ^ aes_x_times(*a3, 2);

        *a0 = r0;
        *a1 = r1;
        *a2 = r2;
        *a3 = r3;
    }
}

void aes_add_round_key(unsigned char * state, unsigned char * round_key) {
    for (int i = 0; i < STATE_SIZE; i++)
        *(state + i) = *(state + i) ^ *(round_key + i);
}

bytes_t * aes_encrypt(char *input, char* key, enum aes aes) {
    struct aes_context* ctx = aes_init_context(aes);

    if (!ctx)
        return NULL;

    for (int i = 0; i < 16; i++) {
        ctx->key->data[i] = key[i];
    }

    int ret = aes_key_expansion(ctx);
    if (ret) {
        aes_free_context(&ctx);
        return NULL;
    }

    aes_cipher(input,ctx);

    bytes_t * out = bytes_new(STATE_SIZE);
    if (!out) {
        aes_free_context(&ctx);
        return NULL;
    }

    memcpy(out->data, ctx->state->data, STATE_SIZE);

    aes_free_context(&ctx);

    return out;
}