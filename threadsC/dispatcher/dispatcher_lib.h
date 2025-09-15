#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "base_thread_lib.h"

typedef struct dispatcher dispatcher_t;


dispatcher_t* create_dispatcher();

interface_t* get_dispatcher_functions(dispatcher_t* handle);


#endif //DISPATCHER_H