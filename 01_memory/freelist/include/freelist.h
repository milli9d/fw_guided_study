#pragma once

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* data;
    size_t sz;
    size_t blk_sz;
    /* Frequency tracking for analysis */
    uint8_t** all_blocks;  /* Array of all block pointers */
    uint32_t* alloc_count; /* Alloc count per block */
    uint32_t* free_count;  /* Free count per block */
    size_t num_blocks;     /* Total number of blocks */
    uint32_t max_alloc;    /* Max alloc count for bar scaling */
} freelist_t;

int32_t freelist_reset(freelist_t* list);

void* freelist_alloc(freelist_t* list);
int32_t freelist_free(freelist_t* list, uint8_t* data);
void freelist_print(freelist_t* list);
void freelist_bar_freq_by_addr(freelist_t* list);

#ifdef __cplusplus
}
#endif