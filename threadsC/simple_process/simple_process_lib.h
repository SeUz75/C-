#ifndef SIMPLE_PROCESS_H
#define SIMPLE_PROCESS_H

#include <stdint.h>
#include "base_thread_lib.h"

#define SIMPLE_MESSAGE_SIZE 11

void* create_simple_process();

interface_t* simple_process_get_interface(void* handle);

#endif