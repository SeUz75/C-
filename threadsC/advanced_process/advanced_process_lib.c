#include <stdlib.h>
#include "advanced_process_lib.h"



// advanced_process_lib.c
typedef struct advanced_process {
    
    base_thread_t* base;     // Pointer works with opaque types
    interface_t* vtable;      // Our own vtable
    size_t supported_n;
    uint32_t* supported[ADVANCED_MESSAGES_SIZE];

};

void advanced_process(void* instance, uint32_t id);
void advanced_destroy(void* instance);
void advanced_send_msg(void* instance, uint32_t id);
void advanced_get_support_messages(void* instance, uint32_t** msg, size_t* msg_size);

interface_t advanced_VTABLE = {
    .send_msg = advanced_send_msg,
    .get_supported_msg = advanced_get_support_messages,
    .destroy = advanced_destroy,
};

// Override methods
void advanced_send_msg(void* instance, uint32_t id) {
    printf("advanced PROCESS: Custom send_msg for ID %u\n", id);
    advanced_process_t* advanced = (advanced_process_t*)instance;
    
    // Delegate to base thread
    interface_t* base_interface = base_get_interface_functions(advanced->base);
    base_interface->send_msg(advanced->base, id);  // Pass base, not advanced
}

advanced_process_t* create_advanced_process() {
    advanced_process_t* advanced = (advanced_process_t*) malloc(sizeof(advanced_process_t));
    if (!advanced) return NULL;
    
    // Create base thread
    advanced->base = base_thread_create(advanced, advanced_process);
    if (!advanced->base) {
        free(advanced);
        return NULL;
    }

    advanced->supported_n = ADVANCED_MESSAGES_SIZE;

    for (size_t i=0; i<11; i++){    
        advanced->supported[i] = (uint32_t)(i+10);
    }
    
    // Set up our vtable with overrides
    advanced->vtable = &advanced_VTABLE;
    
    return advanced;
}

void advanced_process(void* instance, uint32_t id){
    if(id >= 30 && id <=40){
        printf("advanced_PROCESS message %u\n", id);
    }
    else{
        printf("Message cant be processed by advanced PROCESS \n");
    }
}

void advanced_get_support_messages(void* instance, uint32_t** msg, size_t* msg_size){
    advanced_process_t* advanced_instance = instance;
    *msg = advanced_instance->supported;  // ✅ Single dereference
    *msg_size = advanced_instance->supported_n;
}

void advanced_destroy(void* instance){
    advanced_process_t* advanced_instance = instance;
    
    // Get base interface and call its destroy
    interface_t* base_interface = base_get_interface_functions(advanced_instance->base);
    base_interface->destroy(advanced_instance->base);
    
    free(advanced_instance);
}

interface_t* advanced_process_get_interface(advanced_process_t* advanced) {
    return advanced->vtable;  // Return our custom interface
}