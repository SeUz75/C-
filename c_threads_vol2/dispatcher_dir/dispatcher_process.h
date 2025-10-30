#include "base_thread.h"

#ifndef DISPATCHER_LIB_H
#define DISPATCHER_LIB_H

void* create_dispatcher();

interface_t* get_dispatcher_functions(void* instance);

#endif // DISPATCHER_LIB_H