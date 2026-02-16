
#include <string>
#include <format>
#include <mutex>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <logging.h>

LOG_TAG(FREELIST);
#include <freelist.hpp>

void freeList::print_usage() {
    if (!_head) {
        return;
    }

    std::lock_guard _lk(_mtx);
    std::string out;
    out.resize(_blk_cnt);
    std::fill(out.begin(), out.end(), '|');
    uintptr_t* curr = _head;
    while (curr) {
        uintptr_t next = *curr;
        int blk_id = ((uint8_t*) curr - _data) / _blk_sz;
        out[blk_id] = '_';
        curr = (uintptr_t*) next;
    }

    LOG_DBG("USAGE: %s", out.c_str());
}

void freeList::print() {
    if (!_head) {
        return;
    }

    std::lock_guard _lk(_mtx);
    uintptr_t* curr = _head;
    while (curr) {
        uintptr_t next = *curr;
        /* print out the current node [block] */
        std::string out = "";
        for (uintptr_t i = (uintptr_t) curr; i < (uintptr_t) ((uint8_t*) curr + _blk_sz); i++) {
            out += std::format("{:02X} ", *(uint8_t*) i);
        }
        int blk_id = ((uint8_t*) curr - _data) / _blk_sz;
        LOG_DBG("#%03d [ %p ] : %s", blk_id, curr, out.c_str());
        curr = (uintptr_t*) next;
    }
}

void* freeList::malloc() {
    std::lock_guard _lk(_mtx);
    if (!_head) {
        // LOG_ERR("No more memory available");
        return nullptr;
    }

    /* return head and move head to next node */
    void* out = _head;
    _head = (uintptr_t*) *_head;
    return out;
}

void freeList::free(void* data) {
    std::lock_guard _lk(_mtx);
    if (!data) {
        return;
    }

    uintptr_t* new_head = (uintptr_t*) data;
    *new_head = (uintptr_t) _head;
    _head = new_head;
}

freeList::freeList(uint8_t* data, size_t sz, size_t blk_sz) : _data(data), _sz(sz), _blk_sz(blk_sz) {
    /* invariant checks */
    if (!_data) {
        throw std::runtime_error("Invalid data memory pointer");
    }
    if (_sz == 0 || _blk_sz == 0) {
        throw std::runtime_error("Invalid block size or size.");
    }

    if (_sz % _blk_sz != 0 || blk_sz < sizeof(uintptr_t)) {
        throw std::runtime_error(
            "Invalid block size [>= sizeof(uintptr_t)] or size [should be a multiple of block size]");
    }

    std::lock_guard _lk(_mtx);
    memset(_data, 0u, _sz);

    /* populate initial list */
    for (int i = 0; i < (int) sz; i += blk_sz) {
        uintptr_t* curr_node_next = (uintptr_t*) &data[i];
        uintptr_t val_next = 0u;
        if (i + blk_sz < sz) {
            val_next = (uintptr_t) ((uint8_t*) curr_node_next + blk_sz);
        }
        *curr_node_next = val_next;
    }

    _blk_cnt = _sz / _blk_sz;
    _head = (uintptr_t*) &data[0u];
    LOG_PASS("Created List: Data : %p Size : %zu Blk_Sz : %zu", _data, _sz, _blk_sz);
}
