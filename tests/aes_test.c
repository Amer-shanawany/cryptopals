#include "bytes.h"
#include <aes.h>
#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <aes.h>


void test_aes_state_new_free() {
    char * string = "AAAABBBBCCCCDDDD";
    bytes_t *input = bytes_new_from_string(string, 16);
    struct aes_state * state = aes_state_new( input);
    CU_ASSERT_PTR_NOT_NULL_FATAL(state);

    CU_ASSERT_STRING_EQUAL(state->arr, string)

    aes_state_free(&state);
    CU_ASSERT_PTR_NULL_FATAL(state);
}

CUNIT_CI_RUN("test_aes",
    CUNIT_CI_TEST(test_aes_state_new_free)
)