#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <algorithm>

#include <cstdio>
#include <cstdlib>

class buffer_pool {
  private:
#pragma pack(push, 1)
    class node_t {
      public:
        node_t* next;
        size_t blk_sz;
        uint8_t in_use;
        uint8_t data;
    };
#pragma pack(pop)

    uint8_t* _data;
    size_t _sz;
    node_t* _head;
    std::recursive_mutex _mtx;

    void coalesce_right(node_t* node);
    void full_coalesce(node_t* node);

  public:
    void print();

    void* malloc(size_t sz);

    void free(void* data);

    buffer_pool(uint8_t* data, size_t sz);
};
