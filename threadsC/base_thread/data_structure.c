#include <inttypes.h>
#include <stdlib.h>

#include "data_structure.h"


node_t* create_queue(node_t* head){
    head = malloc(sizeof(node_t));
    if (head != NULL) {
        head->next = NULL;  // Initialize to NULL
        head->id = 0;       // Optional: initialize id too
    }
    return head;
}

int is_empty(node_t* head){
    if (head == NULL || head->next == NULL){
        return 1; // true
    }
    return 0 ; // false
}

void enqueue(node_t* head, uint32_t value){
    node_t* current = head;

    while (current->next != NULL){
        current = current->next;
    }
    current->next = malloc (sizeof(node_t));
    current->next->id = value;
    current->next->next = NULL;
}

int dequeue(node_t** head){
    if (*head == NULL || (*head)->next == NULL){  // Empty queue
        return -1;
    }
    
    node_t* node_to_remove = (*head)->next;       // ✅ First data node
    uint32_t retval = node_to_remove->id;         // ✅ Get its value
    (*head)->next = node_to_remove->next;         // ✅ Skip over it
    free(node_to_remove);                         // ✅ Free it
    
    return retval;
}

void free_list(node_t* head){
    node_t* temp;
    
    while (head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}
