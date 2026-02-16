#include <stddef.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "freelist.h"

void freelist_print(freelist_t* list) {
    if (!list) {
        return;
    }

    if (!list->data) {
        return;
    }

    if (list->sz < sizeof(uint8_t*) || list->blk_sz == 0u) {
        return;
    }

    uintptr_t curr = (uintptr_t) list->data;
    while (curr != (uintptr_t) NULL) {
        printf("[%p]->", (void*) curr);
        uintptr_t next = *((uintptr_t*) curr);
        curr = next;
    }
    printf(" NULL\n");
}

void freelist_bar_freq_by_addr(freelist_t* list) {
    if (!list || !list->all_blocks || !list->alloc_count || !list->free_count || list->num_blocks == 0) {
        return;
    }

    static int header_drawn = 0;
    
    /* Draw header only once */
    if (!header_drawn) {
        printf("\033[2J\033[H");
        printf("\033[?25l");  /* Hide cursor */
        printf("\nBlock Frequency Analysis (all %zu blocks):\n\n", list->num_blocks);
        printf("%-18s | %-6s | %-5s | %s\n", "Address", "Status", "Index", "Allocations");
        printf("%-18s | %-6s | %-5s | %s\n", "------------------", "------", "-----", "-------------------------------");
        fflush(stdout);
        header_drawn = 1;
    }
    
    /* Position cursor to start of data section (line 6) */
    printf("\033[6;1H");
    fflush(stdout);

    /* Determine if block is currently free or allocated */
    uint8_t* is_free = (uint8_t*) malloc(list->num_blocks);
    if (!is_free) {
        return;
    }
    
    /* Mark all blocks as allocated initially */
    for (size_t i = 0; i < list->num_blocks; i++) {
        is_free[i] = 0;
    }
    
    /* Walk the free list and mark free blocks */
    uintptr_t curr = (uintptr_t) list->data;
    while (curr != (uintptr_t) NULL) {
        for (size_t i = 0; i < list->num_blocks; i++) {
            if (curr == (uintptr_t) list->all_blocks[i]) {
                is_free[i] = 1;
                break;
            }
        }
        uintptr_t next = *((uintptr_t*) curr);
        curr = next;
    }

    /* Print all blocks in order */
    size_t allocated_count = 0;
    size_t currently_free = 0;
    uint32_t total_allocs = 0;
    uint32_t total_frees = 0;

    /* Find max alloc count for bar scaling - recalculate each frame for aggressive resizing */
    uint32_t max_alloc = 1;
    for (size_t i = 0; i < list->num_blocks; i++) {
        if (list->alloc_count[i] > max_alloc) {
            max_alloc = list->alloc_count[i];
        }
    }
    
    /* Add 10% padding to max for better visualization */
    max_alloc = max_alloc + (max_alloc / 10);

    for (size_t i = 0; i < list->num_blocks; i++) {
        char status_char = is_free[i] ? '_' : 'x';
        
        /* Generate bar graph */
        const int bar_width = 30;
        int filled = (list->alloc_count[i] * bar_width) / max_alloc;
        
        printf("0x%016lx | %-6c | %5zu | [", (uintptr_t) list->all_blocks[i], status_char, i);
        for (int j = 0; j < bar_width; j++) {
            printf("%c", j < filled ? '=' : ' ');
        }
        printf("] %u\n", list->alloc_count[i]);
        fflush(stdout);

        if (is_free[i]) {
            currently_free++;
        } else {
            allocated_count++;
            total_allocs += list->alloc_count[i];
        }
    }

    printf("%-18s | %-6s | %-5s | %s\n", "------------------", "------", "-----", "-------------------------------");
    printf("\nCurrently Allocated: %zu blocks\n", allocated_count);
    printf("Currently Free:      %zu blocks\n", currently_free);
    printf("Total Allocations:   %u\n", total_allocs);
    fflush(stdout);

    free(is_free);
}

int32_t freelist_reset(freelist_t* list) {
    if (!list) {
        return -EINVAL;
    }

    if (!list->data) {
        return -EINVAL;
    }

    if (list->sz < sizeof(uint8_t*) || list->blk_sz == 0u) {
        return -EINVAL;
    }

    /* Calculate number of blocks and initialize tracking */
    size_t num_blocks = list->sz / list->blk_sz;
    list->num_blocks = num_blocks;
    list->max_alloc = 1;  /* Initialize bar scaling base */
    
    /* Allocate tracking arrays if not already allocated */
    if (!list->all_blocks) {
        list->all_blocks = (uint8_t**) malloc(num_blocks * sizeof(uint8_t*));
        list->alloc_count = (uint32_t*) malloc(num_blocks * sizeof(uint32_t));
        list->free_count = (uint32_t*) malloc(num_blocks * sizeof(uint32_t));
        
        if (!list->all_blocks || !list->alloc_count || !list->free_count) {
            printf("Error: Unable to allocate tracking memory\n");
            return -EINVAL;
        }
    }
    
    /* Initialize all block pointers and counters */
    for (size_t i = 0; i < num_blocks; i++) {
        list->all_blocks[i] = &list->data[i * list->blk_sz];
        list->alloc_count[i] = 0;
        list->free_count[i] = 0;
    }

    size_t count = 0u;
    for (size_t i = 0u; i < list->sz; i += list->blk_sz) {
        /* place next at start of every block */
        uintptr_t* out = (uintptr_t*) &list->data[i];
        uintptr_t next = (uintptr_t) (&list->data[i] + list->blk_sz);
        if (i + list->blk_sz >= list->sz) {
            next = (uintptr_t) NULL;
        }

        *out = next;
        /* printf("Block %zu Curr = %p Next = %p\n", count, out, (void*)next); */
        count++;
    }

    /* printf("List Inited : Size = %zu Blk Sz = %zu Num blocks = %d Data = %p\n", list->sz, list->blk_sz, count,
           list->data); */
    return 0;
}

void* freelist_alloc(freelist_t* list) {
    if (!list) {
        return NULL;
    }

    if (!list->data) {
        return NULL;
    }

    if (list->sz < sizeof(uint8_t*) || list->blk_sz == 0u) {
        return NULL;
    }

    /* if list is not null then give back head and move head to next */
    uintptr_t curr = (uintptr_t) list->data;
    uintptr_t next = *((uintptr_t*) list->data);

    /* Track allocation in frequency map */
    if (list->all_blocks && list->alloc_count) {
        for (size_t i = 0; i < list->num_blocks; i++) {
            if ((uintptr_t) list->all_blocks[i] == curr) {
                list->alloc_count[i]++;
                break;
            }
        }
    }

    list->data = (uint8_t*) next;
    return (void*) curr;
}

int32_t freelist_free(freelist_t* list, uint8_t* data) {
    if (!list) {
        return -EINVAL;
    }

    if (list->sz < sizeof(uint8_t*) || list->blk_sz == 0u) {
        return -EINVAL;
    }

    /* Track free in frequency map */
    if (list->all_blocks && list->free_count) {
        for (size_t i = 0; i < list->num_blocks; i++) {
            if ((uintptr_t) list->all_blocks[i] == (uintptr_t) data) {
                list->free_count[i]++;
                break;
            }
        }
    }

    /* attach to head and point next to curr head */
    uintptr_t curr_head = (uintptr_t) list->data;
    list->data = data;
    uintptr_t* to_write = (uintptr_t*) list->data;
    *to_write = curr_head;

    return 0;
}