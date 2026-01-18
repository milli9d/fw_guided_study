#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ring_buffer.h"

int32_t init_ring_buffer(ring_buffer_t* rb, size_t size) {
    rb->in = 0;
    rb->out = 0;
    rb->cap = size;
    rb->size = 0u;
    rb->data = (uint8_t*) calloc(sizeof(uint8_t), rb->cap);
    if (!rb->data) {
        printf("ERROR: No more memory!\n");
        return -ENOMEM;
    }
    return 0;
}

size_t ring_buffer_free_space(ring_buffer_t* rb) {
    return rb->cap - rb->size;
}

void ring_buffer_print(const ring_buffer_t* rb) {
    for (size_t i = 0u; i < rb->cap; i++) {
        printf("%d", rb->data[i]);
        if (i == rb->in) {
            printf("{IN}");
        }
        if (i == rb->out) {
            printf("{OUT}");
        }
        printf(" | ");
    }
    printf("\n");
}

int32_t ring_buffer_put(ring_buffer_t* rb, uint8_t* data, size_t len) {
    /* check if we have free space */
    if (ring_buffer_free_space(rb) < len) {
        printf("ERROR: Not enough space in ring buffer!\n");
        return -ENOMEM;
    }

    size_t size_till_end = rb->cap - rb->in - 1u;
    size_t to_write = MIN(size_till_end, len);
    memcpy(rb->data + rb->in, data, to_write);
    rb->in = (rb->in + to_write) % rb->cap;

    if (to_write < len) {
        size_t rem = len - to_write;
        memcpy(rb->data + rb->in, data + to_write, rem);
        rb->in = (rb->in + rem) % rb->cap;
    }

    rb->size += len;
    return 0;
}

int32_t ring_buffer_get(ring_buffer_t* rb, uint8_t* data_out, size_t len_requested) {
    if (!rb || !data_out) {
        return -EINVAL;
    }

    size_t to_read_total = MIN(len_requested, rb->size);

    size_t data_till_end = rb->cap - rb->out - 1u;
    size_t to_read = MIN(to_read_total, data_till_end);
    memcpy(data_out, rb->data + rb->out, to_read);
    rb->out = (rb->out + to_read) % rb->cap;

    if (to_read < to_read_total) {
        size_t rem = to_read_total - to_read;
        memcpy(data_out + to_read, rb->data + rb->out, rem);
        rb->out = (rb->out + rem) % rb->cap;
    }

    rb->size -= to_read_total;
    return to_read_total;
}