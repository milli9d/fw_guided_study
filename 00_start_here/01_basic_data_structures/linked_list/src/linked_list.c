#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

#include <linked_list.h>

#include "logging.h"
LOG_TAG(linked_list);

node_t* make_node(uint32_t val, struct node_t* next) {
    node_t* out = (node_t*) calloc(sizeof(node_t), 1u);
    out->data = val;
    out->next = next;
    return out;
}

size_t list_size(const node_t* head) {
    size_t count = 0u;
    while (head) {
        head = head->next;
        count++;
    }
    return count;
}

void list_print(const node_t* head) {
    while (head) {
        printf("[%du]->", head->data);
        head = head->next;
    }
    printf("\n");
}

void list_append(uint8_t val, node_t** head) {
    if (!head) {
        LOG_ERR("Invalid list pointer.");
        return;
    }
    *head = make_node(val, *head);
}

void list_push_back(uint8_t val, node_t** head) {
    if (!head) {
        LOG_ERR("Invalid list pointer.");
        return;
    }

    if (!*head) {
        list_append(val, head);
        return;
    }

    node_t* ptr = *head;
    while (ptr && ptr->next) { ptr = ptr->next; }
    ptr->next = make_node(val, NULL);
}

void list_reverse(node_t** head) {
    if (!head) {
        LOG_ERR("Invalid list pointer.");
        return;
    }

    if (!*head) {
        return;
    }

    node_t* curr = *head;
    node_t* prev = NULL;
    while (curr) {
        node_t* link_fw = curr->next;

        curr->next = prev;

        prev = curr;
        curr = link_fw;
    }

    *head = prev;
}

void list_insert(uint8_t val, size_t idx, node_t** head) {
    if (!head) {
        LOG_ERR("Invalid list pointer.");
        return;
    }

    size_t size = list_size(*head);
    if (idx > size) {
        LOG_ERR("Invalid index.");
        return;
    }

    if (idx == 0u) {
        list_append(val, head);
        return;
    }

    node_t* ptr = *head;
    while (idx-- > 1u && ptr) { ptr = ptr->next; }
    ptr->next = make_node(val, ptr->next);
}