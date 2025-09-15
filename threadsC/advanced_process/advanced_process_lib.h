#ifndef ADVANCED_PROCESS_H
#define ADVANCED_PROCESS_H

#include <stdint.h>
#include "base_thread_lib.h"

#define ADVANCED_MESSAGES_SIZE 11

typedef struct advanced_process advanced_process_t;

advanced_process_t* create_advanced_process();

interface_t* advanced_process_get_interface(advanced_process_t* handle);

#endif