#include <stddef.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "buffer_pool.hpp"

#include "logging.h"
LOG_TAG(buffer_pool);

void buffer_pool::print() {
    std::lock_guard _lk(_mtx);
    node_t* curr = _head;
    while (curr) {
        LOG_DBG("[%p] Size : %4zu Next : %12p In Use : %d", curr, curr->blk_sz, curr->next, curr->in_use);
        curr = curr->next;
    }
}

void buffer_pool::coalesce_right(node_t* node) {
    std::lock_guard _lk(_mtx);
    node_t* curr = node->next;
    while (curr && !curr->in_use) {
        node->blk_sz += ((sizeof(node_t) - 1u) + curr->blk_sz);
        node->next = curr->next;
        curr = curr->next;
    }
}

void buffer_pool::full_coalesce(node_t* node) {
    std::lock_guard _lk(_mtx);
    while (node) {
        if (!node->in_use) {
            coalesce_right(node);
        }
        node = node->next;
    }
}

void* buffer_pool::malloc(size_t sz) {
    LOG_DBG("Malloc Request : %zu bytes", sz);
    if (!_head) {
        LOG_ERR("No more space");
        return nullptr;
    }

    if (sz == 0) {
        LOG_ERR("Invalid Size");
        return nullptr;
    }

    std::lock_guard _lk(_mtx);
    full_coalesce(_head);

    /* find next node that will fit */
    node_t* curr = _head;
    while (curr) {
        if (!curr->in_use) {
            /* handle block already exists */
            if (curr->blk_sz == sz || curr->blk_sz == sz + sizeof(node_t) - 1u) {
                LOG_DBG("Found fit [%p] Size : %zu", curr, curr->blk_sz);
                curr->in_use = 1;
                return &curr->data;
            }
            /* handle making a new block */
            else if (curr->blk_sz >= sz + sizeof(node_t) - 1u) {
                break;
            }
        }
        curr = curr->next;
    }

    if (!curr) {
        LOG_ERR("Out of space");
        return nullptr;
    }

    LOG_DBG("Splitting node [%p] Size : %zu", curr, curr->blk_sz);

    /* save og node data */
    size_t og_size = curr->blk_sz; // 50
    node_t* og_next = curr->next;
    LOG_DBG("Original Node [%p] Size : %zu Next : %12p In Use : %d", curr, curr->blk_sz, curr->next, curr->in_use);

    /* shrink this node to fit */
    curr->blk_sz = sz; // 32
    curr->in_use = 1u;
    curr->next = nullptr;

    node_t* next_node = (node_t*) ((uint8_t*) &curr->data + curr->blk_sz);
    bool in_bounds =
        (((uint8_t*) next_node + sizeof(node_t)) < (_data + _sz)) && ((uint8_t*) next_node + sizeof(node_t) > _data);
    if (in_bounds) {
        curr->next = next_node;
        next_node->in_use = 0u;
        next_node->next = og_next;
        next_node->blk_sz = og_size - curr->blk_sz - (sizeof(node_t) - 1u);
        LOG_DBG("In bounds for new node [%p] Size : %zu", next_node, next_node->blk_sz);
    }
    LOG_DBG("After Split [%p] Size : %zu Next : %12p In Use : %d", curr, curr->blk_sz, curr->next, curr->in_use);
    LOG_DBG("New Node [%p] Size : %zu Next : %12p In Use : %d", next_node, next_node->blk_sz, next_node->next,
            next_node->in_use);
    return &curr->data;
}

void buffer_pool::free(void* data) {
    if (!data) {
        LOG_ERR("Invalid Pointer");
        return;
    }

    std::lock_guard _lk(_mtx);
    node_t* node = reinterpret_cast<node_t*>((uint8_t*) data - offsetof(node_t, data));
    if (((uint8_t*) node > _data + _sz) || ((uint8_t*) node < _data)) {
        LOG_ERR("Out of range");
        return;
    }

    LOG_DBG("Freeing node [%p] Size : %zu Data : %p", node, node->blk_sz, data);
    node->in_use = 0;

    full_coalesce(_head);
}

buffer_pool::buffer_pool(uint8_t* data, size_t sz) : _data(data), _sz(sz) {
    if (!_data) {
        throw std::runtime_error("Invalid data pointed");
    }

    if (sz == 0) {
        throw std::runtime_error("Invalid Size");
    }

    std::lock_guard _lk(_mtx);

    /* initialize first node */
    _head = (node_t*) _data;
    _head->next = (node_t*) nullptr;
    _head->blk_sz = _sz - sizeof(node_t);
    _head->in_use = 0;
    memset(data + sizeof(node_t), 0u, _head->blk_sz);
}
