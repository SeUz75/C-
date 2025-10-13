#include <stdlib.h>
#include "simple_process_lib.h"

static uint32_t SUPPORTED_MESSAGES[] = {
       20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
       50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
};

// simple_process_lib.c
typedef struct simple_process {
    void* base;     // Pointer works with opaque types
    interface_t vtable;      // Our own vtable
    size_t supported_n;
    uint32_t* supported;
}simple_process_t;

void simple_process(void* instance, uint32_t id);
void simple_destroy(void* instance);
void simple_send_msg(void* instance, uint32_t id);
void simple_get_support_messages(void* instance, uint32_t** msg, size_t* msg_size);


// Override methods
void simple_send_msg(void* instance, uint32_t id) {
    simple_process_t* simple = (simple_process_t*)instance;
    
    // Delegate to base thread
    interface_t* base_interface = base_get_interface_functions(simple->base);
    base_interface->send_msg(simple->base, id);  // Pass base, not simple
}

void* create_simple_process() {
    simple_process_t* simple = (simple_process_t*) malloc(sizeof(simple_process_t));
    if (!simple) return NULL;
    
    // Create base thread
    simple->base = base_thread_create(simple, simple_process);
    if (!simple->base) {
        free(simple);
        return NULL;
    }
    
    simple->supported = (uint32_t*) &SUPPORTED_MESSAGES;
    simple->supported_n = sizeof(SUPPORTED_MESSAGES)/sizeof(uint32_t);
    


    simple->vtable.init = create_simple_process;
    simple->vtable.get_supported_msg = simple_get_support_messages;
    simple->vtable.send_msg = simple_send_msg;
    simple->vtable.destroy = simple_destroy;
    
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
    
    // printf("Size of supported messages: %zu\n", simple_instance->supported_n);
    // printf("Messages are:\n");
    // for (size_t i = 0; i < simple_instance->supported_n; i++) {
    //     printf("Message [%zu] -> %u\n", i, simple_instance->supported[i]);  
    // }
    
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

interface_t* simple_process_get_interface(void* simple) {
    simple_process_t* simple_instance = simple;
    
    return &simple_instance->vtable;  // Return our custom interface
}