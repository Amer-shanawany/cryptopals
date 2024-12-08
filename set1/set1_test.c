#include "CUnit/CUnit.h"
#include "CUnit/CUnitCI.h"
#include "set1.h"
#include <ctype.h>

static void test_challenge1()
{
    char* the_string = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char* expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    char* hex = hex_to_ascii(the_string);
    CU_ASSERT_PTR_NOT_NULL(hex);

    char* result = char_to_base64(hex);
    CU_ASSERT_PTR_NOT_NULL(result);

    CU_ASSERT_STRING_EQUAL(expected, result);

    free(hex);
    free(result);
}

static void test_challenge2()
{
    char* buffer1 = "1c0111001f010100061a024b53535009181c";
    char* buffer2 = "686974207468652062756c6c277320657965";
    char* expected = "746865206B696420646F6E277420706C6179";

    char* result_ascii = fixed_xor(buffer1, buffer2);
    CU_ASSERT_PTR_NOT_NULL(result_ascii);

    printf("result_ascii %s\n", result_ascii);
    char* result_hex = ascii_to_hex_string(result_ascii);
    CU_ASSERT_PTR_NOT_NULL(result_hex);

    printf("result_hex %s\n", result_hex);
    CU_ASSERT_STRING_EQUAL(expected, result_hex);
    free(result_ascii);
    free(result_hex);
}

CUNIT_CI_RUN("set1",
    CUNIT_CI_TEST(test_challenge1),
    CUNIT_CI_TEST(test_challenge2));
