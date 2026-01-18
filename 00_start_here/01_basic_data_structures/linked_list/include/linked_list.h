#pragma once

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

typedef struct node_t {
    struct node_t* next;
    uint32_t data;
} node_t;

node_t* make_node(uint32_t val, struct node_t* next);

size_t list_size(const node_t* head);

void list_print(const node_t* head);

void list_reverse(node_t** head);

void list_append(uint8_t val, node_t** head);

void list_push_back(uint8_t val, node_t** head);

void list_insert(uint8_t val, size_t idx, node_t** head);
