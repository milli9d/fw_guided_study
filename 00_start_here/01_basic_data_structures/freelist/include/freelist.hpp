#pragma once

#include <iostream>
#include <algorithm>
#include <memory>
#include <mutex>

class freeList {
  private:
    uint8_t* _data;
    size_t _sz;
    size_t _blk_sz;
    size_t _blk_cnt;

    uintptr_t* _head;
    std::mutex _mtx;

  public:
    void print();
    void print_usage();

    void* malloc();
    void free(void* data);

    freeList(uint8_t* data, size_t sz, size_t blk_sz);
};