#include <ctime>
#include <cstdint>
#include <errno.h>
#include <stdint.h>
#include <cassert>
#include <stdlib.h>

#include <thread>
#include <chrono>
#include <vector>

#include "logging.h"
LOG_TAG(freelist_tb);

#include "freelist.h"

static uint8_t _mem[512u] = {};

int main() {
    LOG_INFO("Hello World");
    static freelist_t list = { .data = _mem,
                               .sz = sizeof(_mem),
                               .blk_sz = 32u,
                               .all_blocks = NULL,
                               .alloc_count = NULL,
                               .free_count = NULL,
                               .num_blocks = 0,
                               .max_alloc = 1 };

    freelist_reset(&list);
    std::vector<uintptr_t> _memo;
    int num_blocks = list.sz / list.blk_sz;
    srand(time(0));
    
    while (1) {

        for (int i = 0; i < (rand() % num_blocks); i++) {
            uint8_t* datum = (uint8_t*) freelist_alloc(&list);
            if (datum) {
                _memo.push_back((uintptr_t) datum);
                freelist_bar_freq_by_addr(&list);
            }
        }

        for (int i = 0; i < (rand() % num_blocks); i++) {
            /* start de-allocs */
            if (_memo.empty()) {
                break;
            }
            int itr = rand() % _memo.size();
            uint8_t* to_free = (uint8_t*) _memo.at(itr);
            _memo.erase(_memo.begin() + itr);
            if (to_free) {
                freelist_free(&list, to_free);
                freelist_bar_freq_by_addr(&list);
            }
        }
    }

    return 0;
}