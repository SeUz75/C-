#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "base_thread_lib.h"
#include "data_structure.h"
#include "simple_process_lib.h"

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

    void* simple_handle = dlopen("/home/seuz75/C/thread2/install/lib/libsimple_process.so", RTLD_LAZY);

    if (!simple_handle) {
        printf("Error loading library: %s\n", dlerror());
        return 1;
    }

     // Get function pointers
    simple_process_t* (*create_simple)() = dlsym(simple_handle, "create_simple_process");
    interface_t* (*get_interface)(simple_process_t*) = dlsym(simple_handle, "simple_process_get_interface");

    if (!create_simple || !get_interface) {
        printf("Error getting symbols: %s\n", dlerror());
        dlclose(simple_handle);
        return 1;
    }

    simple_process_t* simple_instance = create_simple();
    if (!simple_instance) {
        printf("Failed to create simple process\n");
        dlclose(simple_handle);
        return 1;
    }

    interface_t* simple_functions = get_interface(simple_instance);

    simple_functions->send_msg(simple_instance, 15);

    simple_functions->destroy(simple_instance);
    dlclose(simple_handle);
    
    
    return 0;
}