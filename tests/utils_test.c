#include <bytes.h>
#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <utils.h>

static void test_hamming_distance() {
    struct test_case {
        char *first;
        char *second;
        int distance;
    };

    struct test_case cases[] = {
        {"A", "B",  2},
        {"karolin", "kathrin",9},
        {"00000","11111",5}
    };

    int total = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < total; i++) {
        bytes_t *first = bytes_new_from_string(cases[i].first,strlen(cases[i].first));
        bytes_t *second = bytes_new_from_string(cases[i].second, strlen(cases[i].second));
        CU_ASSERT_PTR_NOT_NULL(first);
        CU_ASSERT_PTR_NOT_NULL(second);

        int expected = cases[i].distance;
        int distance = hamming_distance(first, second);
        CU_ASSERT_EQUAL(distance, expected);

        bytes_free(&first);
        bytes_free(&second);
        CU_ASSERT_PTR_NULL(first);
        CU_ASSERT_PTR_NULL(second);
    }
}


static void test_hamming_distance_not_equal_length() {
    int distance = hamming_distance(NULL, NULL);
    int expected = 0;
    CU_ASSERT_EQUAL(distance, expected);

    bytes_t a;
    bytes_t b;
    a.data = NULL;
    distance = hamming_distance(&a, &b);
    CU_ASSERT_EQUAL(distance, expected);

    bytes_t *first = bytes_new(1);
    bytes_t *second = bytes_new(1);
    first->length = 0;
    distance = hamming_distance(first, second);
    CU_ASSERT_EQUAL(distance, expected);
    first->length = 1;

    bytes_free(&second);
    second = bytes_new(2);
    distance = hamming_distance(first, second);
    CU_ASSERT_EQUAL(distance, expected);

    bytes_free(&first);
    bytes_free(&second);
}


static void test_bytes_bigram_analysis() {
    char *test = "OMOMOM";
    bytes_t *t = bytes_new_from_string(test, strlen(test));

    float weight = bytes_bigram_analysis(t);
    CU_ASSERT_DOUBLE_EQUAL(weight, 15.4 * 3, 1);
}

CUNIT_CI_RUN("test_utils",
    CUNIT_CI_TEST(test_hamming_distance),
    CUNIT_CI_TEST(test_hamming_distance_not_equal_length),
    CUNIT_CI_TEST(test_bytes_bigram_analysis)
);
