#include <vector>
#include <ranges>
#include <thread>
#include <chrono>

#include <logging.h>
LOG_TAG(MAIN);

#include <freelist.hpp>

static uint8_t data_mem[4096u];
static freeList list(data_mem, sizeof(data_mem), 32u);

static void _run() {
    list.print();
    while (1) {
        std::vector<void*> fetches;
        for (int i = 0; i < rand() % 32u; i++) {
            // LOG_INFO("MALLOC");
            uint8_t* data = (uint8_t*) list.malloc();
            if (data) {
                fetches.push_back((void*) data);
                // list.print();
            }
        }

        for (auto i: fetches) {
            // LOG_INFO("FREE");
            list.free(i);
            // list.print();
            list.print_usage();
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10u));
        }
    }
}

int main() {
    LOG_PASS("SUCKS_ASS");

    std::vector<std::thread> _pool;
    srand(time(0));
    for (int i = 0; i < 32u; i++) { _pool.push_back(std::thread(_run)); }

    std::this_thread::sleep_for(std::chrono::milliseconds::max());
    return 0;
}