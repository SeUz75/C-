#include <stdlib.h>
#include <dlfcn.h>
#include "dispatcher_process.h"

#define FOUND_IN_SIMPLE (1)
#define FOUND_IN_ADVANCED (1)

typedef struct dispatcher_process
{
    interface_t dispatcher_functions;
    uint32_t* dispatcher_supported_messages;
    size_t dispatcher_count_msgs;


    void* simple_instance;
    void* advanced_instance;

    interface_t* simple_functions;
    interface_t* advanced_functions;

    uint32_t* simple_msgs;
    size_t simple_count_msgs;

    uint32_t* advanced_msgs;
    size_t advanced_count_msgs;

}dispatcher_process_t;


    // void* (* init)(); TICK
    // void (* get_supported_msg)(void* instance, uint32_t** msg, size_t* msg_size); TICK
    // void (* send_msg)(void* instance, uint32_t msg); TICK
    // void (* destroy)(void* instance);

void dispatcher_get_supported_messages(void* instance, uint32_t** msg, size_t* msg_size) { 
    dispatcher_process_t* dispatcher_instance = (dispatcher_process_t*) instance;
    // printf("Merged messages in dispatcher are : \n");
    // for (size_t i = 0; i < dispatcher_instance->dispatcher_count_msgs; i++) {
    //     printf("Dispatcher -> %d \n", dispatcher_instance->dispatcher_supported_messages[i]);
    // }
    *msg = dispatcher_instance->dispatcher_supported_messages;
    *msg_size = dispatcher_instance->dispatcher_count_msgs;

}


void dispatcher_send_message(void* instance, uint32_t id) {
    dispatcher_process_t* dispatcher_instance = (dispatcher_process_t*) instance;
    int found_in_simple = 0;
    int found_in_advanced = 0;

    // printf("Simple supported messages are : \n");                                  
    // for (size_t i = 0; i < dispatcher_instance->simple_count_msgs; i++) { 
    //     printf("Simple -> %d \n", dispatcher_instance->simple_msgs[i]);
    // }

    for (size_t i = 0; i < dispatcher_instance->simple_count_msgs; i++) {
        if (id == dispatcher_instance->simple_msgs[i]) {
            found_in_simple = FOUND_IN_SIMPLE;
            break;
        }
    }

    for (size_t i = 0; i < dispatcher_instance->advanced_count_msgs; i++) {
        if (id == dispatcher_instance->advanced_msgs[i]) {
            found_in_advanced = FOUND_IN_ADVANCED;
            break;
        }
    }

    if (found_in_simple) {
        dispatcher_instance->simple_functions->send_msg(dispatcher_instance->simple_instance, id);
    }
    if (found_in_advanced) {
        dispatcher_instance->advanced_functions->send_msg(dispatcher_instance->advanced_instance, id);
    }
    if (!found_in_simple && !found_in_advanced) {
        printf("Message %d not supported by any process\n", id);
    }
}


void dispatcher_destroy(void* instance) {
    dispatcher_process_t* dispatcher_instance = (dispatcher_process_t*) instance;

    dispatcher_instance->simple_functions->destroy(dispatcher_instance->simple_instance);
    dispatcher_instance->advanced_functions->destroy(dispatcher_instance->advanced_instance);

    free(dispatcher_instance->dispatcher_supported_messages);
    free(dispatcher_instance);
}

void* create_dispatcher(){
    dispatcher_process_t* dispatcher_instance = (dispatcher_process_t*) calloc(1, sizeof(dispatcher_process_t));

    if (dispatcher_instance == NULL) {
        printf("Could not allocate memory for dispatcher instance \n");
        return NULL;
    }

    // Simple handle
    void* simple_handle = dlopen("/home/tarnished/work/C-/threadsC/c_threads_vol2/install/lib/libc_simple.so", RTLD_LAZY);
    if (simple_handle == NULL) {
        printf("Could not find the library, failed dlopen simple\n");
        free(dispatcher_instance);
        return NULL;
    }

    // Creating simple instance and getting its functions
    void* (*create_simple_process)() =(void* (*)()) dlsym(simple_handle, "create_simple_process");
    interface_t* (*get_simple_functions)(void*) =(interface_t* (*)(void*)) dlsym(simple_handle, "get_simple_functions");
    if (!create_simple_process || !get_simple_functions) {
        printf("Error getting simple symbols %s \n", dlerror());
        dlclose(simple_handle);
        free(dispatcher_instance);
        return NULL;
    }

    // Using the function pointers to get simple attributes
    dispatcher_instance->simple_instance = create_simple_process();
    dispatcher_instance->simple_functions = get_simple_functions(dispatcher_instance->simple_instance);


    // Advanced handle
    void* advanced_handle = dlopen("/home/tarnished/work/C-/threadsC/c_threads_vol2/install/lib/libc_advanced.so", RTLD_LAZY);
    if (advanced_handle == NULL) {
        printf("Could not find the library, failed dlopen advanced\n");
        free(dispatcher_instance);
        return NULL;
    }

    // Creating advanced instance and getting its functions
    void* (*create_advanced_process)() =(void* (*)()) dlsym(advanced_handle, "create_advanced_process");
    interface_t* (*get_advanced_functions)(void*) =(interface_t* (*)(void*)) dlsym(advanced_handle, "get_advanced_functions");
    if (!create_advanced_process || !get_advanced_functions) {
        printf("Error getting advanced symbols %s \n", dlerror());
        dlclose(advanced_handle);
        free(dispatcher_instance);
        return NULL;
    }


    // Using the function pointers to get simple attributes
    dispatcher_instance->advanced_instance = create_advanced_process();
    dispatcher_instance->advanced_functions = get_advanced_functions(dispatcher_instance->advanced_instance);

    dispatcher_instance->simple_functions->get_supported_msg(
                                            dispatcher_instance->simple_instance, 
                                            &dispatcher_instance->simple_msgs, 
                                            &dispatcher_instance->simple_count_msgs);

    dispatcher_instance->advanced_functions->get_supported_msg(
                                            dispatcher_instance->advanced_instance, 
                                            &dispatcher_instance->advanced_msgs, 
                                            &dispatcher_instance->advanced_count_msgs);
    // printf("Simple supported messages are : \n");                                  
    // for (size_t i = 0; i < dispatcher_instance->simple_count_msgs; i++) { 
    //     printf("Simple -> %d \n", dispatcher_instance->simple_msgs[i]);
    // }

    // printf("Advanced supported messages are : \n");
    // for (size_t i = 0; i < dispatcher_instance->advanced_count_msgs; i++) { 
    //     printf("Advanced -> %d \n", dispatcher_instance->advanced_msgs[i]);
    // }

    dispatcher_instance->dispatcher_count_msgs = dispatcher_instance->simple_count_msgs + dispatcher_instance->advanced_count_msgs;

    dispatcher_instance->dispatcher_supported_messages = calloc(dispatcher_instance->dispatcher_count_msgs, sizeof(uint32_t));
    for (size_t i = 0; i < dispatcher_instance->simple_count_msgs; i++)
    {   
        dispatcher_instance->dispatcher_supported_messages[i] = dispatcher_instance->simple_msgs[i];
    }
    for (size_t i = 0; i < dispatcher_instance->advanced_count_msgs; i++)
    {   
        dispatcher_instance->dispatcher_supported_messages[dispatcher_instance->simple_count_msgs+i] = dispatcher_instance->advanced_msgs[i];
    }
    

    dispatcher_instance->dispatcher_functions.init = NULL;
    dispatcher_instance->dispatcher_functions.get_supported_msg = dispatcher_get_supported_messages;
    dispatcher_instance->dispatcher_functions.send_msg = dispatcher_send_message;
    dispatcher_instance->dispatcher_functions.destroy = dispatcher_destroy;

    return dispatcher_instance;
}

interface_t* get_dispatcher_functions(void* instance) {
    dispatcher_process_t* dispatcher_instance = (dispatcher_process_t*) instance;
    return &dispatcher_instance->dispatcher_functions;
}
