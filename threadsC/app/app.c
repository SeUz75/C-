#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "dispatcher_lib.h"

#if defined(__ANDOROID__) || defined(ANDROID)
    #include <android/log.h>
    #define MY_PRINTF(fmt, ...) __andoird_log_print(ANDROID_LOG_INFO, "Mytag", fmtm ##__VA_ARGS__)
#else
    #define MY_PRINTF(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

void print_list(node_t* head){
    node_t* current = head->next;

    while (current != NULL){
        printf("%d\n", current->id);
        current = current->next;
    }
}


int main(int argc, char* argv[]) {
    void* dispatcher_handle = dlopen("/home/seuz75/C/assignemt_thread_c/install/lib/libdispatcher.so", RTLD_LAZY);

    if (!dispatcher_handle) {
        printf("Error loading library: %s\n", dlerror());
        return 1;
    }

    // Get function pointers
    void* (*create_dispatcher)(); 
    create_dispatcher = dlsym(dispatcher_handle, "create_dispatcher");

    interface_t* (*get_interface)(void*); 
    get_interface = dlsym(dispatcher_handle, "get_dispatcher_functions");
    printf("get_dispatcher function done \n");
    
    if (!create_dispatcher || !get_interface) {
        printf("Error getting symbols: %s\n", dlerror());
        dlclose(dispatcher_handle);
        return 1;
    }
    
    void* dispatcher_instance = create_dispatcher();

    if (!dispatcher_instance) {
        printf("Failed to create simple process\n");
        dlclose(dispatcher_handle);
        return 1;
    }
    
    interface_t* dispatcher_functions = get_interface(dispatcher_instance);

    uint32_t* msg_arrays;
    size_t n_arrays;
    dispatcher_functions->get_supported_msg(dispatcher_instance, &msg_arrays, &n_arrays);

    // Iterate through both sets
    printf("Supported messages are : \n");
    for (size_t i = 0; i < n_arrays; ++i) {
        printf("Message -> %d\n", msg_arrays[i]);
    }

    
    char* possible_char = NULL;
    uint32_t id = 0;

    for ( uint32_t i = 1; i < argc; i++){
        id = strtoul(argv[i], &possible_char, 10);

        dispatcher_functions->send_msg(dispatcher_instance, id);
        if ( id == 0 || *possible_char !=  '\0'){
            printf("Failure \n");
            break;
        }
    }

    printf("ur so cool lol\n");
    dispatcher_functions->destroy(dispatcher_instance);

    dlclose(dispatcher_handle);
    
    
    return 0;
}