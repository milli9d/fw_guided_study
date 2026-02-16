#include <ctime>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <errno.h>

#include <thread>
#include <chrono>
#include <vector>

#include "logging.h"
LOG_TAG(freelist_tb);

#include "buffer_pool.hpp"

static uint8_t _mem[1024u];
static buffer_pool pool(_mem, sizeof(_mem));

static void run() {
    while (1) {
        std::vector<uint8_t*> data_mem;
        for (int i = 0; i < 16; i++) {
            LOG_INFO("MALLOC");
            uint8_t* mem = (uint8_t*) pool.malloc(((rand() % 16u) + 1u) * 32u);
            if (mem) {
                data_mem.push_back(mem);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50u));
            pool.print();
        }

        for (auto i: data_mem) {
            LOG_INFO("FREE");
            pool.free(i);
            pool.print();
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50u));
        }
    }
}

int main() {
    LOG_INFO("Hello World");

    pool.print();

    srand(time(0));

    std::vector<std::thread> _threads;
    for (int i = 0; i < 16; i++) { _threads.push_back(std::thread(run)); }

    for (auto& i: _threads) { i.join(); }

    // pool.malloc(32);
    // pool.print();

    return 0;
}