#include <stdio.h>
#include <stdlib.h>

#include "simple_process.h"

static uint32_t MESSAGES[] = {
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
};

typedef struct simple_process
{
    interface_t simple_functions;

    void* base_instance;
    interface_t* base_functions;
    
    size_t msg_count;

}simple_process_t;


void simple_send_msg(void* instance, uint32_t id) {
    simple_process_t* simple_instance = (simple_process_t*) instance;
    simple_instance->base_functions->send_msg(simple_instance->base_instance, id);
}

void simple_get_supported_msg(void* instance, uint32_t** msgs, size_t* msg_count) { 
    simple_process_t* simple_instance = (simple_process_t*) instance;
    (*msgs) = MESSAGES;
    *msg_count = simple_instance->msg_count;
}

void simple_process(void* instance, uint32_t id) {
    simple_process_t* simple_instance = (simple_process_t*) instance;
    int found = 0;
    for (size_t i = 0; i < simple_instance->msg_count; i++) {
        if (id == MESSAGES[i]) {
            printf("SIMPLE PROCESS -> %d \n", id);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Message %d cannot be processed by simple_process \n", id);
    }
}

void simple_destroy (void* instance) {
    simple_process_t* simple_instance = (simple_process_t*) instance;
    simple_instance->base_functions->destroy(simple_instance->base_instance);
    free(simple_instance);
}


void* create_simple_process() {
    simple_process_t* simple_instance = calloc(1, sizeof(simple_process_t));
    if (simple_instance == NULL) {
        printf("Could not allocate memory for advanced process \n");
        return NULL;
    }

    simple_instance->base_instance = base_init(simple_instance, simple_process);
    if (simple_instance->base_instance == NULL) {
        free(simple_instance);
        return NULL;
    }
    simple_instance->base_functions = base_thread_functions(simple_instance->base_instance);
    simple_instance->msg_count = sizeof(MESSAGES)/sizeof(uint32_t);

    simple_instance->simple_functions.init = NULL;
    simple_instance->simple_functions.get_supported_msg = simple_get_supported_msg;
    simple_instance->simple_functions.send_msg = simple_send_msg;
    simple_instance->simple_functions.destroy = simple_destroy;

    return simple_instance;
}

interface_t* get_simple_functions(void* instance) {
    simple_process_t* simple_instance = (simple_process_t*) instance;
    return &simple_instance->simple_functions;
}
