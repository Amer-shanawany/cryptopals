#include "CUnit/CUnit.h"
#include "CUnit/CUnitCI.h"
#include "set1.h"

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

CUNIT_CI_RUN("set1", CUNIT_CI_TEST(test_challenge1));
