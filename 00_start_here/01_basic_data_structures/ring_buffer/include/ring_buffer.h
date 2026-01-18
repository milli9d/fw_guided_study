#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX(x, y) (x >= y ? x : y)
#define MIN(x, y) (x <= y ? x : y)

typedef struct {
    size_t cap;
    size_t size;
    size_t in;
    size_t out;
    uint8_t* data;
} ring_buffer_t;

int32_t init_ring_buffer(ring_buffer_t* rb, size_t size);

size_t ring_buffer_free_space(ring_buffer_t* rb);

int32_t ring_buffer_put(ring_buffer_t* rb, uint8_t* data, size_t len);

int32_t ring_buffer_get(ring_buffer_t* rb, uint8_t* data_out, size_t len_requested);

void ring_buffer_print(const ring_buffer_t* rb);