#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <print>
#include <memory>

#include <cstdlib>
#include <stdexcept>

#include "logging.h"

class ring_buffer {
  private:
    size_t _in{ 0u };
    size_t _out{ 0u };

    size_t _size{ 0u };
    size_t _cap{ 0u };

    std::unique_ptr<uint8_t[]> _data{ nullptr };

  public:
    void print() {
        for (size_t i = 0u; i < _cap; i++) {
            printf("0x%X", _data[i]);
            if (i == _in) {
                printf("{IN}");
            }
            if (i == _out) {
                printf("{OUT}");
            }
            printf(" | ");
        }
        printf("\n");
    }

    template<typename T>
    void push(const T& data) {
        size_t len = sizeof(T);
        if (len > free_space()) {
            throw std::runtime_error("Out of space");
        }

        size_t space_till_end = _cap - _in;
        size_t to_write = std::min(len, space_till_end);
        std::print("Space {} to_write {}\n", space_till_end, to_write);
        memcpy(&_data[_in], (void*) &data, to_write);
        _in += to_write;

        if (to_write < len) {
            size_t rem = len - to_write;
            memcpy(&_data[_in], (uint8_t*) &data + rem, len);
            _in += rem;
        }

        _size += len;
    }

    template<typename T>
    T pop() {
        T out{};
        uint8_t* out_ptr = (uint8_t*) &out;
        for (size_t i = 0; i < sizeof(T); i++) {
            *out_ptr++ = _data[_out];
            _out = (_out + 1u) % _cap;
        }
        _size -= sizeof(T);
        return out;
    }

    size_t free_space() {
        return _cap - _size;
    }

    ring_buffer(size_t size_bytes) : _cap(size_bytes) {
        _data = std::make_unique<uint8_t[]>(_cap);
    }
};