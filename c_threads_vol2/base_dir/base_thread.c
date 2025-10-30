#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "base_thread.h"
#include "data_structure.h"

typedef struct base_thread
{
    interface_t base_functions;

    pthread_t thread;
    pthread_mutex_t mtx;
    pthread_cond_t cv;

    data_structure_t* queue;
    void* owner;
    process_fn_t callback;

}base_thread_t;

void base_send_msg(void* instance, uint32_t msg) {
    base_thread_t* base_instance = (base_thread_t*) instance;
    pthread_mutex_lock(&base_instance->mtx);
    enqueue_msg(&base_instance->queue, msg);
    pthread_cond_signal(&base_instance->cv);
    pthread_mutex_unlock(&base_instance->mtx);
}

void base_destroy(void* instance){
    base_thread_t* base_instance = (base_thread_t*)instance;
    
    base_instance->base_functions.send_msg(base_instance, 0);

    pthread_join(base_instance->thread, NULL);

    pthread_mutex_lock(&base_instance->mtx);
    free_queue(&base_instance->queue);
    pthread_mutex_unlock(&base_instance->mtx);

    pthread_cond_destroy(&base_instance->cv);
    pthread_mutex_destroy(&base_instance->mtx);

    free(base_instance);
}

void* loop(void* args){
   base_thread_t* base_instance = (base_thread_t*) args;
   while (1) {
        pthread_mutex_lock(&base_instance->mtx);
        while (queue_empty(base_instance->queue) == 1){
            pthread_cond_wait(&base_instance->cv, &base_instance->mtx);
        }
        uint32_t message = dequeue_msg(&base_instance->queue);
        pthread_mutex_unlock(&base_instance->mtx);

        if (message == 0) { 
            break;
        } else {
            base_instance->callback(base_instance->owner, message);
        }
   }
   return NULL;
}

void* base_init(void* instance, process_fn_t process_fn) {
    base_thread_t* base_instance = (base_thread_t*) calloc(1, sizeof(base_thread_t));
    if (base_instance == NULL) {
        printf("Memory could not get allocated \n");
        return NULL;
    }

    if (pthread_mutex_init(&base_instance->mtx, NULL) != 0) {
        printf("Could not initialise mutex \n");
        free(base_instance);
        return NULL;
    }

    if (pthread_cond_init(&base_instance->cv, NULL) !=0) {
        printf("Could not initialise mutex \n");
        pthread_mutex_destroy(&base_instance->mtx);
        free(base_instance);
        return NULL;
    }

    base_instance->queue = NULL;
    base_instance->owner = instance;
    base_instance->callback = process_fn;

    if (pthread_create(&base_instance->thread, NULL, loop, base_instance) !=0) {
        printf("Could not create thread \n");
        pthread_mutex_destroy(&base_instance->mtx);
        pthread_cond_destroy(&base_instance->cv);
        free(base_instance);
        return NULL;
    }

    base_instance->base_functions.init = NULL;
    base_instance->base_functions.get_supported_msg = NULL;
    base_instance->base_functions.send_msg = base_send_msg;
    base_instance->base_functions.destroy = base_destroy;

    return base_instance;
}

interface_t* base_thread_functions(void* instance) {
    base_thread_t* base_instance = (base_thread_t*) instance;
    return &base_instance->base_functions;
}