#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <bytes.h>

void test_bytes_new() {
    size_t length = 256;
    bytes_t* new  = bytes_new(length);
    CU_ASSERT_PTR_NOT_NULL(new);
    CU_ASSERT_PTR_NOT_NULL(new->data);
    CU_ASSERT_EQUAL(new->length, length);
    bytes_free(&new);
}

static void test_bytes_new_invalid() {
    bytes_t* new = bytes_new(0);
    CU_ASSERT_PTR_NULL(new);
}

static void test_bytes_free() {
    size_t length = 256;
    bytes_t* new  = bytes_new(length);
    bytes_free(&new);
    CU_ASSERT_PTR_NULL(new);
}

static void test_bytes_new_from_string() {
    char* string = "ABCDEF";
    bytes_t* new = bytes_new_from_string(string, strlen(string));
    CU_ASSERT_PTR_NOT_NULL(new);
    CU_ASSERT_EQUAL(new->length, strlen(string));
    for (size_t i = 0; i < strlen(string); i++) {
        CU_ASSERT_EQUAL(new->data[i], string[i]);
    }

    bytes_free(&new);
}

static void test_bytes_new_from_string_invalid() {
    bytes_t* new = bytes_new_from_string(NULL, 1);
    CU_ASSERT_PTR_NULL(new);

    char* string = "ABCDEF";
    new          = bytes_new_from_string(string, 0);
    CU_ASSERT_PTR_NULL(new);
}

static void test_hex_string_to_bytes() {
    char* string   = "4142434445";
    char* expected = "ABCDE";
    bytes_t* new   = hex_string_to_bytes(string);
    CU_ASSERT_EQUAL(new->length, strlen(expected));
    for (size_t i = 0; i < strlen(expected); i++) {
        CU_ASSERT_EQUAL(new->data[i], expected[i]);
    }
    bytes_free(&new);
}

static void test_hex_string_to_bytes_invalid() {
    char* invalid_hex = NULL;
    bytes_t* new      = hex_string_to_bytes(invalid_hex);
    CU_ASSERT_PTR_NULL(new);

    invalid_hex = "4";
    new         = hex_string_to_bytes(invalid_hex);
    CU_ASSERT_PTR_NULL(new);

    invalid_hex = "xxxx";
    new         = hex_string_to_bytes(invalid_hex);
    CU_ASSERT_PTR_NULL(new);
}

static void test_bytes_to_hex_string() {
    char* expected_string = "4142434445";
    char* test_string     = "ABCDE";
    bytes_t* test_bytes   = bytes_new(5);

    strncpy((char*)test_bytes->data, test_string, strlen(test_string));
    char* actual_string = bytes_to_hex_string(test_bytes);
    CU_ASSERT_STRING_EQUAL(actual_string, expected_string)

    bytes_free(&test_bytes);
    free(actual_string);
}

static void test_bytes_to_hex_string_invalid() {
    bytes_t* invalid = NULL;
    char* string     = bytes_to_hex_string(invalid);
    CU_ASSERT_PTR_NULL(string);

    invalid = malloc(sizeof(bytes_t));
    memset(invalid, 0, sizeof(bytes_t));

    string = bytes_to_hex_string(invalid);
    CU_ASSERT_PTR_NULL(string);
    free(invalid);

    invalid         = bytes_new(1);
    invalid->length = 0;
    CU_ASSERT_PTR_NULL(string);
    bytes_free(&invalid);
}

static void test_bytes_to_string() {
    char string[] = "ABCDEF";
    bytes_t* b    = bytes_new_from_string(string, strlen(string));
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);

    char* result = bytes_to_string(b);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);

    CU_ASSERT_STRING_EQUAL(result, string);
    free(result);
    bytes_free(&b);
}

static void test_bytes_to_string_invalid() {
    char* result = bytes_to_string(NULL);
    CU_ASSERT_PTR_NULL(result);

    bytes_t b = {0};
    result    = bytes_to_string(&b);
    CU_ASSERT_PTR_NULL(result);

    unsigned char c;
    b.data = &c;

    result = bytes_to_string(&b);
    CU_ASSERT_PTR_NULL(result);
    free(result);
}

void test_bytes_xor() {
    char* expected_string = "abcdef";
    bytes_t* expected     = hex_string_to_bytes(expected_string);

    char* first_string = "414243444546";
    bytes_t* first     = hex_string_to_bytes(first_string);

    char* second_string = "202020202020";
    bytes_t* second     = hex_string_to_bytes(second_string);

    bytes_t* actual = bytes_xor(first, second);

    for (size_t i = 0; i < strlen(expected_string); i++) {
        CU_ASSERT_EQUAL(actual->data[i], expected_string[i]);
    }

    bytes_free(&first);
    bytes_free(&second);
    bytes_free(&actual);
    bytes_free(&expected);
}

static void test_bytes_xor_invalid() {
    bytes_t* invalid = bytes_xor(NULL, NULL);
    CU_ASSERT_PTR_NULL(invalid);

    bytes_t* first = malloc(sizeof(bytes_t));
    memset(first, 0, sizeof(bytes_t));

    invalid = bytes_xor(first, NULL);
    CU_ASSERT_PTR_NULL(invalid);

    bytes_t* second = malloc(sizeof(bytes_t));
    invalid         = bytes_xor(NULL, second);
    CU_ASSERT_PTR_NULL(invalid);

    invalid = bytes_xor(first, second);
    CU_ASSERT_PTR_NULL(invalid);
    free(first);
    free(second);

    first  = bytes_new(1);
    second = bytes_new(2);

    first->length = 0;
    invalid       = bytes_xor(first, second);
    CU_ASSERT_PTR_NULL(invalid);
    first->length = 1;

    second->length = 0;
    invalid        = bytes_xor(first, second);
    CU_ASSERT_PTR_NULL(invalid);
    second->length = 2;

    invalid = bytes_xor(first, second);
    CU_ASSERT_PTR_NULL(invalid);

    second->length = 1;
    invalid        = bytes_xor(first, second);
    CU_ASSERT_PTR_NULL(invalid);

    bytes_free(&first);
    bytes_free(&second);
}

CUNIT_CI_RUN("test_bytes", CUNIT_CI_TEST(test_bytes_new),
             CUNIT_CI_TEST(test_bytes_new_invalid),
             CUNIT_CI_TEST(test_bytes_free),
             CUNIT_CI_TEST(test_bytes_new_from_string),
             CUNIT_CI_TEST(test_bytes_new_from_string_invalid),
             CUNIT_CI_TEST(test_hex_string_to_bytes),
             CUNIT_CI_TEST(test_hex_string_to_bytes_invalid),
             CUNIT_CI_TEST(test_bytes_to_hex_string),
             CUNIT_CI_TEST(test_bytes_to_hex_string_invalid),
             CUNIT_CI_TEST(test_bytes_to_string),
             CUNIT_CI_TEST(test_bytes_to_string_invalid),
             CUNIT_CI_TEST(test_bytes_xor),
             CUNIT_CI_TEST(test_bytes_xor_invalid));
