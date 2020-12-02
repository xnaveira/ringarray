//
// Created by xaviern on 4/27/20.
//

#ifndef ZEPHYR_SANDBOX_RINGARRAY_H
#define ZEPHYR_SANDBOX_RINGARRAY_H

typedef s64_t ring_array_t;
struct ring_array {
    ring_array_t *r;
    int r_size;
    int r_pos;
};


struct ring_array *ringarray_init(size_t size);
void ringarray_push(struct ring_array *ra, ring_array_t el);
int ringarray_peek(struct ring_array *ra, int n, ring_array_t *buffer);
void ringarray_destroy(struct ring_array *ra);
void test_ringarray();

#endif //ZEPHYR_SANDBOX_RINGARRAY_H
