#ifndef SIMPLE_PROCESS_H
#define SIMPLE_PROCESS_H

#include <stdint.h>
#include "base_thread_lib.h"

#define SIMPLE_MESSAGE_SIZE 11

typedef struct simple_process simple_process_t;

simple_process_t* create_simple_process();

interface_t* simple_process_get_interface(simple_process_t* handle);

#endif