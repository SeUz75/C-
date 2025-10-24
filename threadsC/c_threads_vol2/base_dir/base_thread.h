#include <stdio.h>
#include <stdint.h>

#ifndef BASE_THREAD_H
#define BASE_THREAD_H

typedef void (*process_fn_t)(void* instance, uint32_t id);

typedef struct {
    void* (* init)();
    void (* get_supported_msg)(void* instance, uint32_t** msg, size_t* msg_size);
    void (* send_msg)(void* instance, uint32_t msg);
    void (* destroy)(void* instance);
} interface_t;

void* base_init(void* instance, process_fn_t process_fn);

interface_t* base_thread_functions(void* instance);
#endif // BASE_THREAD_H