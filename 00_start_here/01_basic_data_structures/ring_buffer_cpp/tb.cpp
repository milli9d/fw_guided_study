#include <print>

#include "logging.h"
LOG_TAG(main);

#include "ring_buffer.hpp"

int main() {
    ring_buffer rb(32u);

    rb.push<uint32_t>(0xDEADD00D);

    rb.print();

    std::print("0x{:X}\n", rb.pop<uint32_t>());

    return 0;
}