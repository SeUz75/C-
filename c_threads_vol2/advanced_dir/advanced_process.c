#include <stdio.h>
#include <stdlib.h>
#include "advanced_process.h"
static uint32_t MESSAGES[] = {
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
};

typedef struct advanced_process
{
    interface_t advanced_functions;
    void* base_instance;
    interface_t* base_functions;
    size_t msg_count;

}advanced_process_t;

void advanced_send_msg(void* instance, uint32_t id) {
    advanced_process_t* advanced_instance = (advanced_process_t*) instance;
    advanced_instance->base_functions->send_msg(advanced_instance->base_instance, id);
}

void advanced_get_supported_msg(void* instance, uint32_t** msgs, size_t* msg_count) { 
    advanced_process_t* advanced_instance = (advanced_process_t*) instance;
    (*msgs) = MESSAGES;
    *msg_count = advanced_instance->msg_count;
}

void advanced_process(void* instance, uint32_t id) {
    advanced_process_t* advanced_instance = (advanced_process_t*) instance;
    int found = 0;
    for (size_t i = 0; i < advanced_instance->msg_count; i++) {
        if (id == MESSAGES[i]) {
            printf("ADVANCED PROCESS -> %d \n", id);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Message %d cannot be processed by advanced_process \n", id);
    }
}

void advanced_destroy (void* instance) {
    advanced_process_t* advanced_instance = (advanced_process_t*) instance;
    advanced_instance->base_functions->destroy(advanced_instance->base_instance);
    free(advanced_instance);
}


void* create_advanced_process() {
    advanced_process_t* advanced_instance = calloc(1, sizeof(advanced_process_t));
    if (advanced_instance == NULL) {
        printf("Could not allocate memory for advanced process \n");
        return NULL;
    }

    advanced_instance->base_instance = base_init(advanced_instance, advanced_process);
    if (advanced_instance->base_instance == NULL) {
        free(advanced_instance);
        return NULL;
    }
    advanced_instance->base_functions = base_thread_functions(advanced_instance->base_instance);
    advanced_instance->msg_count = sizeof(MESSAGES)/sizeof(uint32_t);

    advanced_instance->advanced_functions.init = create_advanced_process;
    advanced_instance->advanced_functions.get_supported_msg = advanced_get_supported_msg;
    advanced_instance->advanced_functions.send_msg = advanced_send_msg;
    advanced_instance->advanced_functions.destroy = advanced_destroy;

    return advanced_instance;
}

interface_t* get_advanced_functions(void* instance) {
    advanced_process_t* advanced_instance = (advanced_process_t*) instance;
    return &advanced_instance->advanced_functions;
}
