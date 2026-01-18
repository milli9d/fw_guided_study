#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "logging.h"
LOG_TAG(SORT);

#include "sorting.h"

/**
 * @brief Swaps two 32-bit unsigned integers using XOR operations.
 *
 * Performs an in-place swap of two uint32_t values without using a temporary variable.
 * Uses bitwise XOR operations to exchange the values.
 *
 * @param a Pointer to the first uint32_t value to swap.
 * @param b Pointer to the second uint32_t value to swap.
 *
 * @note This function modifies the values at the addresses pointed to by @a a and @b.
 */
void swap(uint32_t* a, uint32_t* b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

/* ========================================================================= */
/* BUBBLE SORT - O[N2] */
/* ========================================================================= */

/**
 * @brief Sorts an array of 32-bit unsigned integers using the bubble sort algorithm.
 *
 * Implements the bubble sort algorithm which repeatedly steps through the array,
 * compares adjacent elements, and swaps them if they are in the wrong order.
 * Continues until no more swaps are needed (array is sorted).
 *
 * @param data Pointer to the array of uint32_t values to sort.
 *             Must not be NULL if len > 0.
 * @param len  The number of elements in the array.
 *
 * @return void
 *
 * @note Time Complexity: O(n²) in worst and average cases, O(n) in best case.
 *       Space Complexity: O(1) - sorts in place.
 * @note If @a data is NULL or @a len is 0, an error is logged and the function returns.
 */
void bubble_sort(uint32_t* data, size_t len) {
    if (!data || len == 0u) {
        LOG_ERR("%s: Invalid data.", __func__);
        return;
    }

    int swaps = 0;
    do {
        swaps = 0;
        for (size_t i = 0u; i < len - 1u; i++) {
            if (data[i + 1] < data[i]) {
                swap(&data[i + 1], &data[i]);
                swaps++;
            }
        }
    } while (swaps > 0);
}

/* ========================================================================= */
/* INSERTION SORT - O[N2] */
/* ========================================================================= */

void insertion_sort(uint32_t* data, size_t len) {
    if (!data || len == 0u) {
        LOG_ERR("%s: Invalid data.", __func__);
        return;
    }

    for (int i = 1; i < (int) len; i++) {
        /* compare curr and prev */
        uint32_t* curr = &data[i];
        uint32_t* prev = &data[i - 1u];

        /* walk back till start of arr */
        int ir = i;
        while (ir-- && *prev > *curr) {
            /* if prev is greater, swap */
            swap(curr, prev);
            curr--;
            prev--;
        }
    }
}

