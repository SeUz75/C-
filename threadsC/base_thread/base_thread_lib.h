// base_thread_lib.h
#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

#include "data_structure.h"

typedef struct base_thread_s base_thread_t;

// Process function typedef
typedef void (*process_fn_t)(void* instance, uint32_t id);

// Interface structure with all interface functions
typedef struct {
    void* (*init)(void* instance);  // Fixed: should take instance parameter
    void (*get_supported_msg)(void* instance, uint32_t** msg, size_t* msg_size);
    void (*send_msg)(void* instance, uint32_t msg);
    void (*destroy)(void* instance);
} interface_t;


// Public functions
void* base_thread_create(void* instance, process_fn_t cb);
interface_t* base_get_interface_functions();

#endif // BASE_THREAD_H