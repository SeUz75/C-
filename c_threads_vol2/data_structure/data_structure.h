#include <stdint.h>

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H



typedef struct data_structure
{
    uint32_t data;
    struct data_structure* next;
    
}data_structure_t;


void enqueue_msg(data_structure_t** queue, uint32_t msg);
uint32_t dequeue_msg(data_structure_t** queue);
int queue_empty(data_structure_t* queue);
void print_queue(data_structure_t* queue);
void free_queue(data_structure_t** queue);

#endif //DATA_STRUCTURE_H