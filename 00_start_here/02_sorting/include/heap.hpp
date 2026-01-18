#pragma once

#include "sorting.h"
#include <cstdbool>

#include <cstddef>
#include <vector>
#include <functional>

#define LEFT(x)   (2u * x) + 1u
#define RIGHT(x)  (2u * x) + 2u
#define PARENT(x) (x - 1u) / 2u

template<typename T>
void sift_up(size_t idx, std::vector<T>& heap, bool max_heap = false) {
    if (heap.size() <= 1u) {
        return;
    }

    std::function compare = [](const T& n1, const T& n2) -> bool { return n1 > n2; };
    if (max_heap) {
        compare = [](const T& n1, const T& n2) -> bool { return n1 < n2; };
    }

    size_t curr = idx;
    size_t parent = PARENT(curr);
    while (curr > 0 && compare(heap[parent], heap[curr])) {
        std::swap(heap[curr], heap[parent]);
        curr = parent;
        parent = PARENT(curr);
    }
}

template<typename T>
void sift_down(size_t idx, std::vector<T>& heap) {
    if (heap.size() <= 1u) {
        return;
    }

    size_t curr = idx;
    while (true) {
        size_t left = LEFT(curr);
        size_t right = RIGHT(curr);
        size_t smallest = curr;

        // Find smallest among curr, left, right
        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }

        // If curr is already smallest, we're done
        if (smallest == curr) {
            break;
        }

        // Swap and continue from the child position
        std::swap(heap[curr], heap[smallest]);
        curr = smallest;
    }
}

template<typename T>
T min_heap_pop(std::vector<T>& heap) {
    /* push to end and sift up */
    T out = heap.front();
    heap.front() = heap.back();
    heap.pop_back();
    sift_down(0u, heap);
    return out;
}

template<typename T>
void min_heap_push(const T& val, std::vector<T>& heap) {
    /* push to end and sift up */
    heap.push_back(val);
    sift_up(heap.size() - 1u, heap);
}

template<typename T>
void min_heapify(std::vector<T>& heap) {
    if (heap.empty()) {
        return;
    }

    // Start from last non-leaf node and work backwards to root
    for (int i = PARENT(heap.size()); i >= 0; i--) { sift_down(i, heap); }
}

/* ========================================================================= */
/* MERGE SORT - O[nLogn] */
/* ========================================================================= */

template<typename T>
void merge(std::vector<T>& arr, size_t left, size_t mid, size_t right) {
    std::vector<T> left_arr(arr.begin() + left, arr.begin() + mid + 1u);
    std::vector<T> right_arr(arr.begin() + mid + 1u, arr.begin() + right + 1u);

    size_t l_idx = 0u;
    size_t r_idx = 0u;
    size_t out_idx = left;

    while (l_idx < left_arr.size() && r_idx < right_arr.size()) {
        if (left_arr[l_idx] < right_arr[r_idx]) {
            arr[out_idx++] = left_arr[l_idx++];
        } else {
            arr[out_idx++] = right_arr[r_idx++];
        }
    }

    while (l_idx < left_arr.size()) { arr[out_idx++] = left_arr[l_idx++]; }
    while (r_idx < right_arr.size()) { arr[out_idx++] = right_arr[r_idx++]; }
}

template<typename T>
void merge_sort(std::vector<T>& arr, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t mid = left + (right - left) / 2u;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

template<typename T>
void merge_sort(std::vector<T>& arr) {
    if (arr.empty()) {
        return;
    }
    merge_sort<T>(arr, 0u, arr.size() - 1u);
}