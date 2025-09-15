#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "dispatcher_lib.h"

void print_list(node_t* head){
    node_t* current = head->next;

    while (current != NULL){
        printf("%d\n", current->id);
        current = current->next;
    }
}


int main() {
    // base_thread_t* thread = base_thread_create(NULL, test_callback);
    // interface_t* base_functions = base_get_interface_functions(thread);
    
    // // Use the interface
    
    
    // sleep(1);  // Let it process
    
    // base_functions->send_msg(thread,10);
    // base_functions->send_msg(thread,20);

    // base_functions->destroy(thread);

    void* dispatcher_handle = dlopen("/home/seuz75/C/thread2/install/lib/libdispatcher.so", RTLD_LAZY);

    if (!dispatcher_handle) {
        printf("Error loading library: %s\n", dlerror());
        return 1;
    }

     // Get function pointers
    dispatcher_t* (*create_dispatcher)() = dlsym(dispatcher_handle, "create_dispatcher");
    interface_t* (*get_interface)() = dlsym(dispatcher_handle, "get_dispatcher_functions");

    if (!create_dispatcher || !get_interface) {
        printf("Error getting symbols: %s\n", dlerror());
        dlclose(dispatcher_handle);
        return 1;
    }

    dispatcher_t* dispatcher_instance = create_dispatcher();
    if (!dispatcher_instance) {
        printf("Failed to create simple process\n");
        dlclose(dispatcher_handle);
        return 1;
    }

    interface_t* dispatcher_functions = get_interface();

    dispatcher_functions->send_msg(dispatcher_instance, 15);
    dispatcher_functions->destroy(dispatcher_instance);

    dlclose(dispatcher_instance);
    
    
    return 0;
}