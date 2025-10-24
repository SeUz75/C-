#include <stdio.h>
#include <stdlib.h>
#include "data_structure.h"

void enqueue_msg(data_structure_t** queue, uint32_t msg) {
    data_structure_t* head = (data_structure_t*)(*queue);

    if (head == NULL) {
        head = (data_structure_t*) calloc(1, sizeof(data_structure_t));
        head->data = msg;
        head->next = NULL;
        *queue = head;
        return;
    }

    while (head->next != NULL) {
        head = head->next;
    }

    head->next = (data_structure_t*) calloc(1, sizeof(data_structure_t));
    head->next->data = msg;
    head->next->next = NULL;

}

uint32_t dequeue_msg(data_structure_t** queue) {
    if ((*queue) == NULL) {
        printf("Queue is empty ! \n");
        return 0;
    }

    uint32_t value = 0;
    data_structure_t* temp = (*queue)->next;
    value = (*queue)->data;
    free((*queue));
    (*queue) = temp;
    return value;
}

int queue_empty(data_structure_t* queue) {
    if (queue == NULL) {
        return 1;
    }
    return 0;
}

void print_queue(data_structure_t* queue) {
    data_structure_t* head = queue;
    while (head !=NULL) {
        printf( "Function called \n"); 
        printf("Element -> %d \n", head->data);
        head = (data_structure_t*) head->next;
    }
}

void free_queue(data_structure_t** queue) {
    while ((*queue) != NULL) {
        data_structure_t* temp = (*queue)->next;
        (*queue)->data = 0;
        free((*queue));
        (*queue) = temp;
    }
}
