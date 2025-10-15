#include <dlfcn.h>
#include <stdlib.h>

#include "advanced_process_lib.h"
#include "simple_process_lib.h"
#include "dispatcher_lib.h"

typedef struct dispatcher {
    void* simple_instance;
    void* advanced_instance;

    interface_t* simple_functions;
    interface_t* advanced_functions;

    uint32_t* msgs_simple;
    uint32_t msg_size_simple;

    uint32_t* msgs_advanced;
    uint32_t msg_size_advanced;

    uint32_t* merged;

    interface_t dispatcher_functions;
}dispatcher_t;

void dispatcher_get_supported_message(void* instance, uint32_t** msgs, uint32_t* msg_size);
void dispatcher_send_msg(void* instance, uint32_t id);
void dispatcher_destroy(void* instance);


void dispatcher_get_supported_message(void* instance, uint32_t** msgs, uint32_t* msg_size) {
    dispatcher_t* dispatcher_instance = instance;
    
    uint32_t total_size = dispatcher_instance->msg_size_advanced + dispatcher_instance->msg_size_simple;
    
    // Allocate memory for merged array
    dispatcher_instance->merged = calloc(total_size, sizeof(uint32_t));
    if (!dispatcher_instance->merged) {
        printf("Failed to allocate memory for merged messages\n");
        *msgs = NULL;
        *msg_size = 0;
        return;
    }
    
    // Copy advanced messages
    for (uint32_t i = 0; i < dispatcher_instance->msg_size_advanced; i++) {
        dispatcher_instance->merged[i] = dispatcher_instance->msgs_advanced[i];
    }
    
    // Copy simple messages
    for (uint32_t i = 0; i < dispatcher_instance->msg_size_simple; i++) {
        dispatcher_instance->merged[dispatcher_instance->msg_size_advanced + i] = dispatcher_instance->msgs_simple[i];
    }
    
    *msgs = dispatcher_instance->merged;
    *msg_size = total_size;
    
    // printf("Messages are  : \n");
    // for (uint32_t i = 0; i < total_size; i++) { 
    //     printf("[%d] -> %d \n", i, dispatcher_instance->merged[i]);
    // }
}


void* create_dispatcher() {
    dispatcher_t* dispatcher_instance = malloc(sizeof(dispatcher_t));
    if (!dispatcher_instance) {
        printf("Memory couldn't get allocated for dispatcher\n");
        return NULL;  
    }

    // Load simple process
    void* simple_handle = dlopen("/home/seuz75/C/assignemt_thread_c/install/lib/libsimple_process.so", RTLD_LAZY);
    if (!simple_handle) {
        printf("Error loading simple library: %s\n", dlerror());
        free(dispatcher_instance);
        return NULL;  
    }

    void* (*create_simple)() = dlsym(simple_handle, "create_simple_process");
    interface_t* (*get_simple_interface)(void*) = dlsym(simple_handle, "simple_process_get_interface");

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
    void* advanced_handle = dlopen("/home/seuz75/C/assignemt_thread_c/install/lib/libadvanced_process.so", RTLD_LAZY);
    if (!advanced_handle) {
        printf("Error loading advanced library: %s\n", dlerror());
        // Clean up simple process first
        dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
        dlclose(simple_handle);
        free(dispatcher_instance);
        return NULL;
    }

    void* (*create_advanced)() = dlsym(advanced_handle, "create_advanced_process");
    interface_t* (*get_advanced_interface)(void*) = dlsym(advanced_handle, "advanced_process_get_interface");

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

    dispatcher_instance->advanced_functions->get_supported_msg(
                                            dispatcher_instance->advanced_instance,
                                            &dispatcher_instance->msgs_advanced,
                                            &dispatcher_instance->msg_size_advanced);

    // printf("Messages pass ADVANED are :");
    // printf("Message size of advanced is : %d \n", dispatcher_instance->msg_size_advanced);
    // for (size_t i = 0; i < dispatcher_instance->msg_size_advanced; i++) { 
    //     printf("ADVANCED : %ld \n", dispatcher_instance->msgs_advanced[i]);
    // }                                       

    dispatcher_instance->simple_functions->get_supported_msg(
                                            dispatcher_instance->simple_instance,
                                            &dispatcher_instance->msgs_simple,
                                            &dispatcher_instance->msg_size_simple);
    
    // printf("Messages pass SIMPLE are :");
    // printf("Message size of simple is : %d \n", dispatcher_instance->msg_size_simple);
    // for (size_t i = 0; i < dispatcher_instance->msg_size_simple; i++) { 
    //     printf("SIMPLE : %ld \n", dispatcher_instance->msgs_simple[i]);
    // }    

    dispatcher_instance->dispatcher_functions.init = NULL;
    dispatcher_instance->dispatcher_functions.send_msg = dispatcher_send_msg;
    dispatcher_instance->dispatcher_functions.get_supported_msg = dispatcher_get_supported_message;
    dispatcher_instance->dispatcher_functions.destroy = dispatcher_destroy;
    return dispatcher_instance;
}

void dispatcher_send_msg(void* instance, uint32_t id){
    dispatcher_t* dispatcher_instance = instance;

    // printf("%d   and   %d \n", dispatcher_instance->msgs_simple[0], dispatcher_instance->msgs_simple[ (dispatcher_instance->msg_size_simple-1) ]);
    // printf("%d   and   %d \n", dispatcher_instance->msgs_simple[0], dispatcher_instance->msgs_simple[ ((dispatcher_instance->msg_size_simple/2) -1) ]);
    // printf("%d   and   %d \n", dispatcher_instance->msgs_advanced[0], dispatcher_instance->msgs_advanced[dispatcher_instance->msg_size_advanced/2]);
    printf("Advanced 50 to 60 \n");
    printf("%d \n", dispatcher_instance->msgs_advanced[(dispatcher_instance->msg_size_advanced / 2)]);
    printf("%d \n", dispatcher_instance->msgs_advanced[dispatcher_instance->msg_size_advanced - 1]);
    printf("-------------------\n");

    printf("simple 50 to 60 \n");
    printf("%d \n", dispatcher_instance->msgs_simple[(dispatcher_instance->msg_size_simple/2)]);
    printf("%d \n", dispatcher_instance->msgs_simple[dispatcher_instance->msg_size_simple - 1]);
    printf("-------------------\n");

    if (id >= dispatcher_instance->msgs_simple[0] && id <= dispatcher_instance->msgs_simple[((dispatcher_instance->msg_size_simple/2) - 1)] ){
        dispatcher_instance->simple_functions->send_msg(dispatcher_instance->simple_instance, id);
    }
    else if (id >= dispatcher_instance->msgs_advanced[0] && id <= dispatcher_instance->msgs_advanced[((dispatcher_instance->msg_size_advanced/2) -1)]){
        dispatcher_instance->advanced_functions->send_msg(dispatcher_instance->advanced_instance, id);
    }
    else if (
             id >= dispatcher_instance->msgs_advanced[(dispatcher_instance->msg_size_advanced / 2)] && 
             id <= dispatcher_instance->msgs_advanced[dispatcher_instance->msg_size_advanced - 1]   &&
             id >= dispatcher_instance->msgs_simple[(dispatcher_instance->msg_size_simple/2)]       &&
             id <= dispatcher_instance->msgs_simple[dispatcher_instance->msg_size_simple - 1] )      {

        printf("id : %d\n", id);
        dispatcher_instance->simple_functions->send_msg(dispatcher_instance->simple_instance, id);
        dispatcher_instance->advanced_functions->send_msg(dispatcher_instance->advanced_instance, id);
    }
    else{
        printf("Message IS NOT SUPPORTED !\n");
    }
}

void dispatcher_destroy(void* instance){
    dispatcher_t* dispatcher_instance = instance;
    free(dispatcher_instance->merged);
    dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
    dispatcher_instance->advanced_functions->destroy(dispatcher_instance->advanced_instance);

    free(dispatcher_instance);
}

interface_t* get_dispatcher_functions(void* handle){
    dispatcher_t* dispatcher_instance = (dispatcher_t*)handle;
    return &dispatcher_instance->dispatcher_functions;
}