#include <zephyr.h>
#include <stdio.h>
#include <stdlib.h>
#include "ringarray.h"
#include <logging/log.h>

LOG_MODULE_REGISTER(ringarray, LOG_LEVEL_DBG);

/**@brief a modulo b  (c % operator is rest operation ie doesn't work well on negative numbers) */
static inline int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

/**@brief Creates a ring array of @var size and returns a pointer to it**/
struct ring_array *ringarray_init(size_t size) {
    struct ring_array *ra = k_malloc(sizeof(struct ring_array));
    ra->r_size = size;
    ra->r = k_malloc(sizeof(ring_array_t)*ra->r_size);
    if (ra->r == NULL) {
        printf("Error allocating memory\n"); //TODO: Logging
        return NULL;
    }
    for (int i=0;i<ra->r_size;i++) {
        ra->r[i] = -1; //-1 means not initialized
    }
    ra->r_pos = 0;
    return ra;
}

/**@brief Pushes and element into the ringarray and increases the pointer in a circular fashion**/
void ringarray_push(struct ring_array *ra, ring_array_t el) {
    ra->r[ra->r_pos] = el;
    ra->r_pos = mod(ra->r_pos + 1,ra->r_size);
}

/**@brief Fetches the last @var n elements pushed into the ringarray and returns them to @var buffer**/
int ringarray_peek(struct ring_array *ra, int n, ring_array_t *buffer) {
    if (n > ra->r_size) {
        printf("Can't return more elements than the size of the ringarray\n");
        return -ENODATA;
    }
    if (buffer == NULL) {
        printf("The buffer is NULL\n");
        return -ENOMEM;
    }
    int temp_pos = mod(ra->r_pos - 1,ra->r_size);
    for (int i=0;i<n;i++) {
        buffer[i] = ra->r[temp_pos];
        temp_pos = mod(temp_pos - 1, ra->r_size);
    }
    return 0;
}


/**@brief Frees the memory for the ringarray**/
void ringarray_destroy(struct ring_array *ra) {
    k_free(ra->r);
    k_free(ra);
}


/**@brief Quick and dirty tests**/
void test_ringarray() {
    ring_array_t test_a[] = {4,6,8,3,2,5};
    int test_a_size = 6;
    bool failed = false;

    struct ring_array *ra = ringarray_init(test_a_size);
    for (int i=0;i<test_a_size;i++) {
        ringarray_push(ra,test_a[i]);
    }

    failed = false;
    for (int i=0;i<test_a_size;i++) {
        if (test_a[i] != ra->r[i]) {
            printf("TEST 1 FAILED\n");
            failed = true;
            break;
        }
    }
    if (!failed) {
        printf("TEST 1 PASSED\n");
    }
    ringarray_destroy(ra);

    ra = ringarray_init(test_a_size/2);
    for (int i=0;i<test_a_size;i++) {
        ringarray_push(ra,test_a[i]);
    }

    ring_array_t all[test_a_size/2];
    ringarray_peek(ra,test_a_size/2,all);
    failed = false;
    for (int i=0;i<test_a_size/2;i++) {
            if (all[i] != test_a[test_a_size-1-i]) {
                printf("TEST 2 FAILED\n");
                failed = true;
                break;
            }
    }
    if (!failed) {
        printf("TEST 2 PASSED\n");
    }

    ring_array_t last_two[2];
    ringarray_peek(ra, 2, last_two);

    failed = false;
    for (int i=0;i<2;i++) {
        if (last_two[i] != test_a[test_a_size-1-i]) {
            printf("TEST 3 FAILED\n");
            failed = true;
            break;
        }
    }
    if (!failed) {
        printf("TEST 3 PASSED\n");
    }

    ringarray_destroy(ra);

    ra = ringarray_init(test_a_size-1);
    for (int i=0;i<test_a_size;i++) {
        ringarray_push(ra,test_a[i]);
    }

    ring_array_t expected_r_4[] = {5,2,3,8};
    ring_array_t expected[] = {5,6,8,3,2};
    failed = false;
    for (int i=0;i<test_a_size-1;i++) {
        if (expected[i] != ra->r[i]) {
            printf("TEST 4 FAILED\n");
            failed = true;
            break;
        }
    }
    if (!failed) {
        printf("TEST 4 PASSED\n");
    }

    ring_array_t last_four[4];
    ringarray_peek(ra, 4, last_four);

    failed = false;
    for (int i=0;i<4;i++) {
        if (expected_r_4[i] != last_four[i]) {
            printf("TEST 5 FAILED\n");
            failed = true;
            break;
        }
    }
    if (!failed) {
        printf("TEST 5 PASSED\n");
    }

    ringarray_destroy(ra);
}