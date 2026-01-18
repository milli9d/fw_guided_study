#include <iostream>
#include <cstdint>

#include <forward_list.hpp>

#include <logging.h>
LOG_TAG(main);

int main() {
    LOG_INFO("Forward List Test");

    forward_list<uint8_t> _list{};
    _list.append(10u);
    _list.push_front(20u);
    _list.print();

    _list.push_back(20u);
    _list.push_back(30u);
    _list.print();

    _list.insert(0u, 0u);
    _list.print();

    _list.insert(1u, 1u);
    _list.print();

    _list.insert(6u, 6u);
    _list.print();

    _list.reverse();
    _list.print();

    _list.remove(0u);
    _list.print();

    _list.remove(2u);
    _list.print();

    _list.remove(4u);
    _list.print();

    return 0;
}
