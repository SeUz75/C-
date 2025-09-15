#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#define ARRAY_MAX_SIZE 10000

#include <stdio.h>
#include <inttypes.h>

typedef struct node{

    uint32_t id;
    struct node* next;

} node_t;

node_t* create_queue(node_t* q);

void enqueue(node_t* q, uint32_t value);

int is_empty(node_t* q);

int dequeue(node_t** q);

int is_full(node_t* q);

void free_list(node_t* q);

#endif //DATA_STRUCTURE_H