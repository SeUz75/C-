#include <stdlib.h>

#include "advanced_process_lib.h"
#include "base_thread_lib.h"


static uint32_t SUPPORTED_MESSAGES[] = {
       30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
       50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
};

typedef struct advanced_process {
   void* base;
   interface_t vtable;
   uint32_t* supported_messages;
   size_t supported_n;
} advanced_process_t;

void advanced_process(void* instance, uint32_t id);
void advanced_destroy(void* instance);
void advanced_send_msg(void* instance, uint32_t id);
void advanced_get_support_messages(void* instance, uint32_t** msg,
                                  size_t* msg_size);

void* create_advanced_process() {
   advanced_process_t* advanced =
       (advanced_process_t*)calloc(1, sizeof(advanced_process_t));
   if (!advanced) {
       return NULL;
   }

   advanced->base = base_thread_create(advanced, advanced_process);
   if (!advanced->base) {
       free(advanced);
       return NULL;
   }

   advanced->supported_n = sizeof(SUPPORTED_MESSAGES)/sizeof(uint32_t);
   advanced->supported_messages = (uint32_t*) &SUPPORTED_MESSAGES;

   advanced->vtable.init = NULL;
   advanced->vtable.send_msg = advanced_send_msg;
   advanced->vtable.get_supported_msg = advanced_get_support_messages;
   advanced->vtable.destroy = advanced_destroy;

   return advanced;
}

void advanced_process(void* instance, uint32_t id) {
    advanced_process_t* advanced_process = (advanced_process_t*)instance;
    if (id >= 30 && id <= 40) {
        printf("ADVANCED_PROCESS : %u\n", id);
    } else if (id >= 50 && id <= 60) {
        printf("ADVANCED_PROCESS : %u\n", id);
    }
}

void advanced_send_msg(void* instance, uint32_t id){
   advanced_process_t* advanced_process = (advanced_process_t*)instance;

   interface_t* base_interface = base_get_interface_functions(advanced_process->base);
   base_interface->send_msg(advanced_process->base, id);
}

void advanced_get_support_messages(void* instance, uint32_t** msg,
                                  size_t* msg_size) {
   advanced_process_t* advanced_instance = (advanced_process_t*)instance;

//    printf("Advanced_process messages %ld \n", advanced_instance->supported_n);
//    printf("Messsages are :\n");
//    for (size_t i = 0; i < advanced_instance->supported_n; i++) {
//        printf("Message is : %d\n", advanced_instance->supported_messages[i]);
//    }
//     printf("-------------------------- \n");
   *msg = (uint32_t*) &SUPPORTED_MESSAGES;
   *msg_size = advanced_instance->supported_n;
}

void advanced_destroy(void* instance) {
   advanced_process_t* advanced_instance = instance;
   interface_t* base_interface = base_get_interface_functions(advanced_instance->base);
   base_interface->destroy(advanced_instance->base);

   free(advanced_instance);
}

interface_t* advanced_process_get_interface(void* advanced) {
   advanced_process_t* advanced_instance = (advanced_process_t*)advanced;
   return &advanced_instance->vtable;
}