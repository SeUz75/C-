#include <dlfcn.h>
#include <stdlib.h>

#include "advanced_process_lib.h"
#include "simple_process_lib.h"
#include "dispatcher_lib.h"

typedef struct dispatcher {
    simple_process_t* simple_instance;
    advanced_process_t* advanced_instance;

    interface_t* simple_functions;
    interface_t* advanced_functions;

    uint32_t** msgs;
    uint32_t* msg_size;

    interface_t* dispatcher_functions;
};

void dispatcher_get_supported_message(void* instance, uint32_t** msgs, uint32_t* msg_size);
void dispatcher_send_msg(void* instance, uint32_t id);
void dispatcher_destroy(void* instance);

interface_t DISPATCHER_FUNCTIONS ={
    .get_supported_msg = NULL,
    .send_msg = dispatcher_send_msg,
    .destroy = dispatcher_destroy,
};

dispatcher_t* create_dispatcher() {
    dispatcher_t* dispatcher_instance = malloc(sizeof(dispatcher_t));
    if (!dispatcher_instance) {
        printf("Memory couldn't get allocated for dispatcher\n");
        return NULL;  
    }

    // Load simple process
    void* simple_handle = dlopen("/home/seuz75/C/thread2/install/lib/libsimple_process.so", RTLD_LAZY);
    if (!simple_handle) {
        printf("Error loading simple library: %s\n", dlerror());
        free(dispatcher_instance);
        return NULL;  
    }

    simple_process_t* (*create_simple)() = dlsym(simple_handle, "create_simple_process");
    interface_t* (*get_simple_interface)(simple_process_t*) = dlsym(simple_handle, "simple_process_get_interface");

    if (!create_simple || !get_simple_interface) {
        printf("Error getting simple symbols: %s\n", dlerror());
        dlclose(simple_handle);
        free(dispatcher_instance);
        return NULL; 
    }

    dispatcher_instance->simple_instance = create_simple();
    if (!dispatcher_instance->simple_instance) {
        printf("Failed to create simple process\n");
        dlclose(simple_handle);
        free(dispatcher_instance);
        return NULL;  
    }

    dispatcher_instance->simple_functions = get_simple_interface(dispatcher_instance->simple_instance);

    // Load advanced process
    void* advanced_handle = dlopen("/home/seuz75/C/thread2/install/lib/libadvanced_process.so", RTLD_LAZY);
    if (!advanced_handle) {
        printf("Error loading advanced library: %s\n", dlerror());
        // Clean up simple process first
        dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
        dlclose(simple_handle);
        free(dispatcher_instance);
        return NULL;
    }

    advanced_process_t* (*create_advanced)() = dlsym(advanced_handle, "create_advanced_process");
    interface_t* (*get_advanced_interface)(advanced_process_t*) = dlsym(advanced_handle, "advanced_process_get_interface");

    if (!create_advanced || !get_advanced_interface) {
        printf("Error getting advanced symbols: %s\n", dlerror());
        dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
        dlclose(simple_handle);
        dlclose(advanced_handle);
        free(dispatcher_instance);
        return NULL;
    }

    dispatcher_instance->advanced_instance = create_advanced();
    if (!dispatcher_instance->advanced_instance) {
        printf("Failed to create advanced process\n");
        dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
        dlclose(simple_handle);
        dlclose(advanced_handle);
        free(dispatcher_instance);
        return NULL; 
    }

    dispatcher_instance->advanced_functions = get_advanced_interface(dispatcher_instance->advanced_instance);

    // Initialize supported messages (combine from both processes)
    dispatcher_instance->msgs = NULL;
    dispatcher_instance->msg_size = 0;
    dispatcher_instance->dispatcher_functions = &DISPATCHER_FUNCTIONS;
    
    return dispatcher_instance;
}

void dispatcher_send_msg(void* instance, uint32_t id){
    dispatcher_t* dispatcher_instance = instance;

    // Handle message ID 0 as BREAK - send to both processes to gracefully shut them down
    if (id == 0) {
        printf("DISPATCHER: Received BREAK signal (0), forwarding to all processes...\n");
        dispatcher_instance->simple_functions->send_msg(dispatcher_instance->simple_instance, id);
        dispatcher_instance->advanced_functions->send_msg(dispatcher_instance->advanced_instance, id);
        return;
    }

    if (id >= 10 && id <= 20){
        dispatcher_instance->simple_functions->send_msg(dispatcher_instance->simple_instance, id);
    }
    else if (id >= 30 && id <= 40){
        dispatcher_instance->advanced_functions->send_msg(dispatcher_instance->advanced_instance, id);
    }
    else if (id >= 50 && id <= 60){
        dispatcher_instance->simple_functions->send_msg(dispatcher_instance->simple_instance, id);
        dispatcher_instance->advanced_functions->send_msg(dispatcher_instance->advanced_instance, id);
    }
    else{
        printf("Message IS NOT SUPPORTED !\n");
    }
}

void dispatcher_destroy(void* instance){
    dispatcher_t* dispatcher_instance = instance;

    dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
    dispatcher_instance->advanced_functions->destroy(dispatcher_instance->advanced_instance);

    free(dispatcher_instance);
}

interface_t* get_dispatcher_functions(dispatcher_t* handle){
    return handle->dispatcher_functions;
}