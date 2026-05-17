#ifndef _AES_H_
#define _AES_H_

#include <bytes.h>

#define STATE_SIZE 16
#define STATE_LENGTH 4
#define STATE_ROWS STATE_LENGTH
#define STATE_COLS STATE_LENGTH

// // the state
// struct aes_state {
//     unsigned char ** ptr;
// };

// struct aes_state * aes_state_new();

struct aes_state {
    unsigned char arr[16];
};

void aes_state_free(struct aes_state ** state);
struct aes_state * aes_state_new(bytes_t * input);


inline unsigned int get_state_row(unsigned int index);
inline unsigned int get_state_column(unsigned int index);
inline unsigned int get_output_index(unsigned int row, unsigned int column);


#endif//_AES_H_