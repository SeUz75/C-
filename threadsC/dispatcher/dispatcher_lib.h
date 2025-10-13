#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "base_thread_lib.h"

void* create_dispatcher();

interface_t* get_dispatcher_functions(void* handle);

#endif //DISPATCHER_H
