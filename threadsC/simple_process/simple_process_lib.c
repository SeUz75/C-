#include <stdlib.h>
#include "simple_process_lib.h"

// simple_process_lib.c
typedef struct simple_process {
    base_thread_t* base;     // Pointer works with opaque types
    interface_t* vtable;      // Our own vtable
    size_t supported_n;
    uint32_t* supported[SIMPLE_MESSAGE_SIZE];
};

void simple_process(void* instance, uint32_t id);
void simple_destroy(void* instance);
void simple_send_msg(void* instance, uint32_t id);
void simple_get_support_messages(void* instance, uint32_t** msg, size_t* msg_size);

interface_t SIMPLE_VTABLE = {
    .send_msg = simple_send_msg,
    .get_supported_msg = simple_get_support_messages,
    .destroy = simple_destroy,
};

// Override methods
void simple_send_msg(void* instance, uint32_t id) {
    simple_process_t* simple = (simple_process_t*)instance;
    
    // Delegate to base thread
    interface_t* base_interface = base_get_interface_functions(simple->base);
    base_interface->send_msg(simple->base, id);  // Pass base, not simple
}

simple_process_t* create_simple_process() {
    simple_process_t* simple = (simple_process_t*) malloc(sizeof(simple_process_t));
    if (!simple) return NULL;
    
    // Create base thread
    simple->base = base_thread_create(simple, simple_process);
    if (!simple->base) {
        free(simple);
        return NULL;
    }

    simple->supported_n = SIMPLE_MESSAGE_SIZE;

    for (size_t i=0; i<11; i++){    
        simple->supported[i] = (uint32_t)(i+10);
    }
    
    // Set up our vtable with overrides
    simple->vtable = &SIMPLE_VTABLE;
    
    return simple;
}

void simple_process(void* instance, uint32_t id){
    // Handle message ID 0 as BREAK signal
    if (id == 0) {
        printf("SIMPLE_PROCESS: Received BREAK signal (0), exiting...\n");
        return;
    }
    
    if(id >= 10 && id <=20){
        printf("SIMPLE_PROCESS : %u\n", id);
    }
    else if(id >= 50 && id <= 60){
        printf("SIMPLE_PROCESS : %u\n", id);
    }
    else{
        printf("Message %u cant be processed by SIMPLE PROCESS \n", id);
    }
}

void simple_get_support_messages(void* instance, uint32_t** msg, size_t* msg_size){
    simple_process_t* simple_instance = instance;
    *msg = simple_instance->supported;
    *msg_size = simple_instance->supported_n;
}

void simple_destroy(void* instance){
    simple_process_t* simple_instance = instance;
    
    // Get base interface and call its destroy
    interface_t* base_interface = base_get_interface_functions(simple_instance->base);
    base_interface->destroy(simple_instance->base);
    
    free(simple_instance);
}

interface_t* simple_process_get_interface(simple_process_t* simple) {
    return simple->vtable;  // Return our custom interface
}