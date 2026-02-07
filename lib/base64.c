#include <base64.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const char base64_encode[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
	'=' };

static const unsigned char base64_decode[] = {
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, /* 0   */
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, /* 8   */
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, /* 16  */
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, /* 24  */
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, /* 32  */
	0x80, 0x80, 0x80, 0x3E, 0x80, 0x80, 0x80, 0x3F, /* 40  */
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, /* 48  */
	0x3C, 0x3D, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, /* 56  */
	0x80, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, /* 64  */
	0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, /* 72  */
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, /* 80  */
	0x17, 0x18, 0x19, 0x80, 0x80, 0x80, 0x80, 0x80, /* 88  */
	0x80, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, /* 96  */
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, /* 104 */
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, /* 112 */
	0x31, 0x32, 0x33, 0x80, 0x80, 0x80, 0x80, 0x80, /* 120 */
};


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

    char* output = (char *) malloc(out_size + 1);
    if (!output)
        return NULL;

    memset(output, '=', out_size);

    for (int len = 0; len < input->length; len = len + 3) {
        char* c = (char *) input->data + len;
        int out_i = len * 8 / 6;

        int first = *c >> 2;
        output[out_i] = base64_encode[first];

        int second = (*c & 0x03) << 4;
        output[out_i + 1] = base64_encode[second];

        if (len + 1 < input->length) {
            c++;
            int second_b = *c >> 4;
            second |= second_b;
        } else
            break;
        output[out_i + 1] = base64_encode[second];

        int third = (*c & 0x0F) << 2;
        output[out_i + 2] = base64_encode[third];
        if (len + 2 < input->length) {
            c++;
            int third_b = *c >> 6;
            third |= third_b;
        } else
            break;
        output[out_i + 2] = base64_encode[third];

        int fourth = (*c & 0x3F);
        output[out_i + 3] = base64_encode[fourth];
    }

    output[out_size] = '\0';

    return output;
}

bytes_t* base64_to_bytes(char *input) {
	if (!input)
		return NULL;

	size_t length = strlen(input);

	if (length % 4 != 0)
		return NULL;

	size_t output_length = 3 * length / 4;

	// check valid encoding chars
	for (int i = 0; i < length; i++) {
		if (input[i] < 'A' && input[i] > 'Z' && input[i] != '+'	&&
			input[i] < 'a' && input[i] > 'z' && input[i] != '/' &&
			input[i] < '0' && input[i] > '9' && input[i] != '=') {

			return NULL;
		}
	}

	bytes_t *output = bytes_new(output_length);
	if (!output)
		return NULL;

	for (size_t i = 0, o = 0; i < length; i += 4, o += 3)  {
		unsigned char first  = base64_decode[input[i    ]];
		unsigned char second = base64_decode[input[i + 1]];
		unsigned char third  = base64_decode[input[i + 2]];
		unsigned char fourth = base64_decode[input[i + 3]];

		output->data[o]     = first << 2 | second >> 4;
		output->data[o + 1] = (second & 0xF) << 4 | third >> 2;
		output->data[o + 2] = (third  & 0x3) << 6 | fourth;
	}

	return output;
}
