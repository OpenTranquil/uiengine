#ifndef __UIENGINE_ASSERT_H__
#define __UIENGINE_ASSERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_NULL(element)    \
    if (element != NULL) {  \
        printf("ASSERT_NULL(failed): '"#element"' is not NULL\n");  \
        exit(1);    \
    }

#define ASSERT_NOT_NULL(element)    \
    if (element == NULL) {  \
        printf("ASSERT_NOT_NULL(failed): '"#element"' is NULL\n");  \
        exit(1);    \
    }

#define ASSERT_STR_EQUAL(ptr, val)  \
    if (strcmp(ptr, val) != 0) {    \
        printf("ASSERT_STR_EQUAL(failed): '"#ptr"' not equal to '"#val"'\n");\
        exit(1);    \
    }

typedef void (*TestFunc)();

#define DEFINE_TEST_CASE(name)    \
    int TEST_CASE_##name();    \
    void TEST_##name() {    \
        printf("TEST("#name"):\t"); \
        if(TEST_CASE_##name() == 0) {   \
            printf("success!"); \
        } \
        printf("\n");   \
    }   \
    __attribute__((used)) __attribute__((section("__TESTCASES,__testcases"))) TestFunc TestFunc##name = &TEST_##name;    \
    int TEST_CASE_##name()

// extern TestFunc __TESTCASES_start;
// extern TestFunc __TESTCASES_stop;

// void RUN_ALL_TESTS() {
//     TestFunc *iter = &__TESTCASES_start;
//     for(; iter < &__TESTCASES_stop; ++iter){
//         (*iter)();
//     }
// }
#endif /* __UIENGINE_ASSERT_H__ */