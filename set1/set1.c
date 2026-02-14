#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <cryptopals.h>

#include <ctype.h>
#include <stdbool.h>
#include <float.h>
#include <stddef.h>


static void test_challenge1()
{
    char* hex_string = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    char* expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    bytes_t* hex_bytes = hex_string_to_bytes(hex_string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(hex_bytes);

    char* result = bytes_to_base64(hex_bytes);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);

    CU_ASSERT_STRING_EQUAL(expected, result);

    bytes_free(&hex_bytes);
    free(result);
}

static void test_challenge2()
{
    char* buffer1 = "1c0111001f010100061a024b53535009181c";
    char* buffer2 = "686974207468652062756c6c277320657965";
    char* expected = "746865206B696420646F6E277420706C6179";

    bytes_t* first = hex_string_to_bytes(buffer1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(first);
    bytes_t* second = hex_string_to_bytes(buffer2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(second);

    bytes_t* xored_bytes = bytes_xor(first, second);
    CU_ASSERT_PTR_NOT_NULL_FATAL(xored_bytes);

    char* result = bytes_to_hex_string(xored_bytes);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);
    CU_ASSERT_STRING_EQUAL(expected, result);

    free(result);
    bytes_free(&xored_bytes);
    bytes_free(&second);
    bytes_free(&first);
}

static void test_challenge3()
{
    char* encrypted = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    bytes_t* bytes = hex_string_to_bytes(encrypted);
    CU_ASSERT_PTR_NOT_NULL_FATAL(bytes);

    unsigned char result_key = 0;
    unsigned int rating = 0;
    char* output = (char*) malloc(bytes->length + 1);

    for (unsigned char key = 0x0; key < 0xff; key++) {
        for (int index = 0; index < bytes->length; index++) {
            output[index] = bytes->data[index] ^ key;
        }
        output[bytes->length + 1] = '\0';

        bool valid = true;
        for (int i = 0; i < strlen(output); i++) {
            if (!isprint(output[i])) {
                valid = false;
                break;
            }
        }
        if (valid) {
            unsigned int current_rate = rate_etaoin_shrdlu(output);
            if (current_rate > rating) {
                result_key = key;
                rating = current_rate;
            }
        }
    }

    for (int index = 0; index < bytes->length; index++) {
        output[index] = bytes->data[index] ^ result_key;
    }
    output[bytes->length + 1] = '\0';
    CU_ASSERT_NOT_EQUAL_FATAL(rating, 0);
    printf(" key: %c, rating: %d, output: %s ", result_key, rating, output);

    free(output);
    free(bytes);
}

static void test_challenge4()
{
    FILE* file = fopen("4.txt", "r");
    if (!file) {
        CU_FAIL("couldn't find the test file 4.txt");
    }

    size_t len = 0;
    ssize_t read = 0;
    char* line = NULL;
    unsigned int global_rating = 0;
    char* best_result = NULL;
    char the_key;

    while ((read = getline(&line, &len, file)) != -1) {
        if (read > 0 && line[read - 1] == '\n')
            line[read - 1] = '\0';

        bytes_t* bytes = hex_string_to_bytes(line);
        CU_ASSERT_PTR_NOT_NULL_FATAL(bytes);

        unsigned char result_key = 0;
        unsigned int rating = 0;
        char output[bytes->length + 1];

        for (unsigned char key = 0x1; key < 0xff; key++) {
            for (int index = 0; index < bytes->length; index++) {
                output[index] = bytes->data[index] ^ key;
            }
            output[bytes->length + 1] = '\0';

            unsigned int current_rate = rate_etaoin_shrdlu(output);
            if (current_rate > rating) {
                result_key = key;
                rating = current_rate;
            }
        }

        if (rating) {
            for (int index = 0; index < bytes->length; index++) {
                output[index] = bytes->data[index] ^ result_key;
            }
            output[bytes->length + 1] = '\0';
            if (rating > global_rating) {
                best_result = strdup(output);
                the_key = result_key;
                global_rating = rating;
            }
        }
        bytes_free(&bytes);
    }
    printf(" key is: %c, decrypted string: %s \n", the_key, best_result);

    free(line);
    fclose(file);
}

static void test_challenge5()
{
    char string[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    char* key = "ICE";
    char* expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    bytes_t* string_bytes = bytes_new_from_string(string, strlen(string));
    bytes_t* key_bytes = bytes_new_from_string(key, strlen(key));

    for (int i = 0, k = 0; i < string_bytes->length; i++, k++) {
        if (k >= key_bytes->length)
            k = 0;

        string_bytes->data[i] ^= key_bytes->data[k];
    }

    char* output = bytes_to_hex_string(string_bytes);

    for (int i = 0; i < strlen(output); i++) {
        output[i] = tolower(output[i]);
        if (output[i] != expected[i])
            printf("hex: %02x output: %c expected: %c\n", i, output[i], expected[i]);
    }
    CU_ASSERT_STRING_EQUAL(expected, output);
}

static void test_challenge6_hamming_distance()
{
    char* first = "this is a test";
    char* second = "wokka wokka!!!";

    bytes_t* first_bytes = bytes_new_from_string(first, strlen(first));
    CU_ASSERT_PTR_NOT_NULL_FATAL(first_bytes);

    bytes_t* second_bytes = bytes_new_from_string(second, strlen(second));
    CU_ASSERT_PTR_NOT_NULL_FATAL(second_bytes);

    int distance = hamming_distance(first_bytes, second_bytes);
    printf("distance is %d\n", distance);
    CU_ASSERT_EQUAL(37, distance);

	bytes_free(&first_bytes);
	bytes_free(&second_bytes);
}

static void test_challenge6()
{
	int ret = -1;
	int KEYSIZE = 0;
	double low_distance = DBL_MAX;
	FILE * file = fopen("6.txt", "r");
	CU_ASSERT_PTR_NOT_NULL_FATAL(file);


	size_t length = 0;
	ssize_t nread = 0;
    ssize_t total = 0;
	char *encrypted = NULL;
	char *line = NULL;

	while ((nread = getline(&line, &length, file)) != -1) {
		if (line[nread - 1] == '\n') {
			line[nread - 1] = '\0';
			nread--;
		}

        char *temp = (char *) realloc(encrypted, total+nread);
		memcpy(temp + total, line, nread);
        encrypted = temp;
        total += nread;
        free(line);
        line = NULL;
        length = 0;
	}
    free(line);

	fclose(file);

	bytes_t *encrypted_bytes = base64_to_bytes(encrypted);
	CU_ASSERT_PTR_NOT_NULL_FATAL(encrypted_bytes);
	free(encrypted);


    for (size_t keysize = 2; keysize <= 40; keysize++) {
		int g_distance = 0;

		for (int i = 0; i < 4; i++) {
            size_t offset = keysize * i * 2;
        
			bytes_t * first_bytes = bytes_new(keysize);
			CU_ASSERT_PTR_NOT_NULL_FATAL(first_bytes);
			memcpy(first_bytes->data, encrypted_bytes->data + offset, keysize);
    
			bytes_t * second_bytes = bytes_new(keysize);
			CU_ASSERT_PTR_NOT_NULL_FATAL(second_bytes);
			memcpy(second_bytes->data, encrypted_bytes->data + offset + keysize, keysize);

			double distance = hamming_distance(first_bytes, second_bytes);
			distance /= keysize;
			g_distance += distance;

			bytes_free(&first_bytes);
			bytes_free(&second_bytes);
		}

		// g_distance /= 4;

		if (g_distance < low_distance) {
			low_distance = g_distance;
			KEYSIZE = keysize;
			printf("low: %f global: %d Keysize: %ld\n", low_distance, g_distance, keysize);
		}
	}

	printf("# keysize: %d, distance: %f\n", KEYSIZE, low_distance);

	bytes_t **blocks = malloc(sizeof(bytes_t *) * KEYSIZE);
	CU_ASSERT_PTR_NOT_NULL_FATAL(blocks);

    size_t block_length = encrypted_bytes->length / KEYSIZE;
	for (int block = 0; block < KEYSIZE; block++) {
		blocks[block] = bytes_new(block_length);
		CU_ASSERT_PTR_NOT_NULL_FATAL(blocks[block]);

		for (int index = 0; index < block_length; index++) {
            size_t offset = block + (index * KEYSIZE);
			unsigned char * src = encrypted_bytes->data + offset;
            unsigned char * dest = blocks[block]->data + index;
			memcpy(dest, src, 1);
		}
	}
    // 0  1  2  3  4 
    // 5  6  7  8  9 
    // 10 11 12 13 14
    // 15 16 17 18 19
    // 20 21 22 23 24

	char * the_key = malloc(KEYSIZE);
	CU_ASSERT_PTR_NOT_NULL_FATAL(the_key);
    memset(the_key, 0x0, KEYSIZE);


	char *output = malloc(block_length + 1);
	CU_ASSERT_PTR_NOT_NULL_FATAL(output);

	for (int block = 0; block < KEYSIZE; block++) {
		unsigned int rating = 0;
		for (unsigned char key = 0x1; key < 0xFF; key++) {

			for (int i = 0; i < blocks[block]->length; i++)
				output[i] = blocks[block]->data[i] ^ key;

            output[block_length] = '\0';
	
			unsigned int current_rating = rate_etaoin_shrdlu(output);
			if (current_rating > rating) {
				the_key[block] = key;
                rating = current_rating;
            }
		}
	}
	free(output);

	printf("the key: ");
	for (int i = 0; i < KEYSIZE; i++)
		printf("0x%x ", the_key[i]);
	printf("\n");

	// decrypting the file
	char * decrypted = malloc(encrypted_bytes->length + 1);
	for (int x = 0, y = 0; x < encrypted_bytes->length; x++, y++) {
		if (y >= KEYSIZE)
			y = 0;
		decrypted[x] = encrypted_bytes->data[x] ^ the_key[y];
	}

	decrypted[encrypted_bytes->length + 1] = '\0';

	printf("decrypted text:\n%s\n", decrypted);
	free(decrypted);
	bytes_free(&encrypted_bytes);

	free(the_key);
	for (int block = 0; block < KEYSIZE; block++)
		bytes_free(&blocks[block]);
	free(blocks);
}

CUNIT_CI_RUN("set1",
    CUNIT_CI_TEST(test_challenge1),
    CUNIT_CI_TEST(test_challenge2),
    CUNIT_CI_TEST(test_challenge3),
    CUNIT_CI_TEST(test_challenge4),
    CUNIT_CI_TEST(test_challenge5),
    CUNIT_CI_TEST(test_challenge6_hamming_distance),
    CUNIT_CI_TEST(test_challenge6));
