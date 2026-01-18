#include "time.h"

#include "linked_list.h"
#include "logging.h"
LOG_TAG(linked_list_tb);

int main() {
    LOG_INFO("Hello World");

    node_t* list = NULL;
    srand(time(0));
    list_push_back(99u, &list);
    list_print(list);

    for (int i = 0; i < 10; i++) { list_append(rand(), &list); }
    list_print(list);

    for (int i = 0; i < 5; i++) {
        list_print(list);
        list_push_back(rand(), &list);
    }
    list_print(list);

    list_reverse(&list);
    list_print(list);

    list_insert(88u, 0u, &list);
    list_print(list);

    list_insert(44u, list_size(list) / 2u, &list);
    list_print(list);

    list_insert(88u, list_size(list), &list);
    list_print(list);

    if (list_size(list) == 15u) {
        LOG_PASS("Passed.");
    }

    return 0;
}