#include "bytes.h"
#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <base64.h>

struct vector {
    char* string;
    char* base64;
};

/* rfc4648: section-10: Test Vectors */
struct vector test_vector[] = {
    {"f",      "Zg=="    },
    {"fo",     "Zm8="    },
    {"foo",    "Zm9v"    },
    {"foob",   "Zm9vYg=="},
    {"fooba",  "Zm9vYmE="},
    {"foobar", "Zm9vYmFy"}
};

static void test_bytes_to_base64() {
    int size = sizeof(test_vector) / sizeof(test_vector[0]);

    for (int i = 0; i < size; i++) {
        bytes_t* b = bytes_new_from_string(test_vector[i].string,
                                           strlen(test_vector[i].string));
        CU_ASSERT_PTR_NOT_NULL_FATAL(b);

        char* result = bytes_to_base64(b);

        CU_ASSERT_PTR_NOT_NULL_FATAL(result);
        CU_ASSERT_EQUAL_FATAL(strlen(result), strlen(test_vector[i].base64));
        CU_ASSERT_STRING_EQUAL_FATAL(result, test_vector[i].base64);

        free(result);
        bytes_free(&b);
    }
}

static void test_bytes_to_base64_empty_string() {
    bytes_t* b          = bytes_new(1);
    unsigned char* temp = b->data;
    b->data             = NULL;

    char* result = bytes_to_base64(b);
    CU_ASSERT_PTR_NULL(result);

    b->data = temp;
    bytes_free(&b);
}

static void test_base64_to_bytes() {
    int size = sizeof(test_vector) / sizeof(test_vector[0]);

    for (int i = 0; i < size; i++) {
        bytes_t* b = base64_to_bytes(test_vector[i].base64);
        CU_ASSERT_PTR_NOT_NULL_FATAL(b);

        char* result = bytes_to_string(b);
        CU_ASSERT_PTR_NOT_NULL_FATAL(result);

        CU_ASSERT_EQUAL(strlen(result), strlen(test_vector[i].string));
        CU_ASSERT_STRING_EQUAL(result, test_vector[i].string);
        free(result);
        bytes_free(&b);
    }
}

static void test_base64_to_bytes_invalid() {
    bytes_t* b = base64_to_bytes(NULL);
    CU_ASSERT_PTR_NULL(b);

    base64_to_bytes("");
    CU_ASSERT_PTR_NULL(b);

    char* invalid_length = "AAA";
    b                    = base64_to_bytes(invalid_length);
    CU_ASSERT_PTR_NULL(b);

    char* invalid_encoding = "AA?";
    b                      = base64_to_bytes(invalid_encoding);
    CU_ASSERT_PTR_NULL(b);
}

CUNIT_CI_RUN("test_base64", CUNIT_CI_TEST(test_bytes_to_base64),
             CUNIT_CI_TEST(test_bytes_to_base64_empty_string),
             CUNIT_CI_TEST(test_base64_to_bytes),
             CUNIT_CI_TEST(test_base64_to_bytes_invalid));
