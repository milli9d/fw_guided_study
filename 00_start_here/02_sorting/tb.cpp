#include <algorithm>
#include <array>
#include <vector>
#include <cstdbool>

#include <chrono>
#include <stdint.h>

#include "heap.hpp"
#include "logging.h"
LOG_TAG(SORT_TB);

#include "sorting.h"

using SortFunc = void (*)(uint32_t*, size_t);

void print_arr(uint32_t* data, size_t len) {
    for (size_t i = 0u; i < len; i++) {
        if (i && i % 16u == 0u) {
            printf("\n");
        }
        printf("%3u ", data[i]);
    }
    printf("\n");
}

bool check_sort(uint32_t* data, size_t len) {
    for (int i = 0; i < (int) len; i++) {
        uint32_t* curr = &data[i];
        uint32_t* prev = &data[i];
        if (*prev > *curr) {
            return false;
        }
    }
    return true;
}

template<size_t ArraySize = 10240u>
void run_sort_test(const char* name, SortFunc sort_fn, const uint32_t* original) {
    LOG_INFO("Running test %s", name);
    std::array<uint32_t, ArraySize> data;
    std::copy(original, original + ArraySize, data.begin());

    auto start = std::chrono::high_resolution_clock::now();
    // print_arr(data.data(), data.size());

    sort_fn(data.data(), data.size());
    // print_arr(data.data(), data.size());

    check_sort(data.data(), data.size()) ? LOG_PASS("PASS") : LOG_FAIL("FAIL");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("%s Time: %ld ms\n\n", name, duration.count());
}

#define ARR_SIZE 999999u

int main() {
    srand(time(0));

    std::array<uint32_t, ARR_SIZE> seed;
    std::generate(seed.begin(), seed.end(), []() -> uint32_t { return rand() % 512u; });
    print_arr(seed.data(), seed.size());

    // run_sort_test<ARR_SIZE>("Bubble Sort", bubble_sort, seed.data());
    // run_sort_test<ARR_SIZE>("Insertion Sort", insertion_sort, seed.data());

    std::vector<uint32_t> _heap;

    for (int i = 0; i < 10; i++) {
        min_heap_push(rand() % 50u, _heap);
        print_arr(_heap.data(), _heap.size());
    }

    printf("\n\n\n");

    for (int i = 0; i < 10; i++) {
        print_arr(_heap.data(), _heap.size());
        uint32_t out = min_heap_pop(_heap);
        printf("%d ", out);
    }
    printf("\n\n\n");

    std::vector<uint32_t> _heap2(ARR_SIZE);
    std::generate(_heap2.begin(), _heap2.end(), []() -> uint32_t { return rand() % 9999u; });

    auto start = std::chrono::high_resolution_clock::now();

    min_heapify<uint32_t>(_heap2);
    // print_arr(_heap2.data(), _heap2.size());

    std::vector<uint32_t> _out;
    size_t heap_size = _heap2.size();
    for (size_t i = 0u; i < heap_size; i++) {
        // if (i % 16u == 0u) {
        //     printf("\n");
        // }
        _out.push_back(min_heap_pop(_heap2));
        // printf("%4d ", out);
    }
    // printf("\n");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("Heap Sort Time: %ld ms\n\n", duration.count());

    LOG_INFO("Merge Sort");
    std::vector<uint32_t> merge_arr(ARR_SIZE);
    std::generate(merge_arr.begin(), merge_arr.end(), []() -> uint32_t { return rand() % 9999u; });
    // print_arr(merge_arr.data(), merge_arr.size());

    start = std::chrono::high_resolution_clock::now();
    merge_sort(merge_arr);
    end = std::chrono::high_resolution_clock::now();
    // print_arr(merge_arr.data(), merge_arr.size());

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("Merge Sort Time: %ld ms\n\n", duration.count());

    LOG_PASS("Done!");
    return 0;
}