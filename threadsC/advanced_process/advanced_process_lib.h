#ifndef ADVANCED_PROCESS_H
#define ADVANCED_PROCESS_H

#include <stdint.h>
#include "base_thread_lib.h"

#define ADVANCED_MESSAGES_SIZE 11

void* create_advanced_process();

interface_t* advanced_process_get_interface(void* handle);

#endif