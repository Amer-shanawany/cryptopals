#include "CUnit/CUnit.h"
#include "CUnit/CUnitCI.h"
#include "set1.h"
#include <ctype.h>
#include <stdbool.h>

static void test_challenge1()
{
    char* hex_string = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    char* expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    bytes_t* hex_bytes = hex_string_to_bytes(hex_string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(hex_bytes);

    char* result = bytes_to_base64(hex_bytes);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);

    CU_ASSERT_STRING_EQUAL(expected, result);

    free_bytes(hex_bytes);
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

    bytes_t* xored_bytes = xor_bytes(first, second);
    CU_ASSERT_PTR_NOT_NULL_FATAL(xored_bytes);

    char* result = bytes_to_hex_string(xored_bytes);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);

    CU_ASSERT_STRING_EQUAL(expected, result);

    free(result);
    free_bytes(xored_bytes);
    free_bytes(second);
    free_bytes(first);
}

static void test_challenge3()
{
    char* encrypted = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    bytes_t* bytes = hex_string_to_bytes(encrypted);
    CU_ASSERT_PTR_NOT_NULL_FATAL(bytes);

    unsigned char result_key = 0;
    unsigned int rating = 0;
    char* output = (char*)malloc(bytes->length + 1);

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

CUNIT_CI_RUN("set1",
    CUNIT_CI_TEST(test_challenge1),
    CUNIT_CI_TEST(test_challenge2),
    CUNIT_CI_TEST(test_challenge3));
