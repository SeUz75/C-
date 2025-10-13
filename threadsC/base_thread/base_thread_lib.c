#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "base_thread_lib.h"
#include "data_structure.h"

// Base thread structure
typedef struct base_thread_s {
    interface_t* base_functions;

    pthread_t tid;
    pthread_mutex_t lock;
    pthread_cond_t cv;
    
    int running;
    node_t* queue;
    process_fn_t cb;
    void* owner;
}base_thread_t;

void* bt_init_void();  
void bt_get_supported_msg(void* handle, uint32_t** msg, size_t* n);
void bt_send_msg(void* handle, uint32_t id);
void bt_destroy(void* handle);
void* worker_loop(void* arg);

static interface_t base_thread_vtable;

void* base_thread_create(void* instance, process_fn_t cb){
    base_thread_t* base_obj = (base_thread_t*) malloc(sizeof(base_thread_t));

    if ( !base_obj ){
        printf("Could not alocate memory \n");
        return NULL;
    }

    if (pthread_mutex_init(&base_obj->lock, NULL) != 0){
        printf("Could not init mutex\n");
        free(base_obj);
        return NULL;
    }

    if (pthread_cond_init(&base_obj->cv, NULL) != 0){
        printf("Could not init cond variable\n");
        pthread_mutex_destroy(&base_obj->lock);
        free(base_obj);
        return NULL;
    }

    base_obj->queue = create_queue(NULL);
    base_obj->running = 1;
    base_obj->owner = instance;
    base_obj->cb = cb;

    if(pthread_create(&base_obj->tid, NULL, worker_loop, base_obj)){
        printf("Could not create thread\n");
        pthread_mutex_destroy(&base_obj->lock);
        pthread_cond_destroy(&base_obj->cv);
        free_list(base_obj->queue);
        free(base_obj);
        return NULL;
    }

    base_thread_vtable.send_msg = bt_send_msg;
    base_thread_vtable.destroy = bt_destroy;
    base_thread_vtable.init = NULL;
    base_thread_vtable.get_supported_msg = NULL;
    base_obj->base_functions = &base_thread_vtable;

    return (void*)base_obj;
}

void bt_send_msg (void* handle, uint32_t id){
    base_thread_t* base_obj = (base_thread_t*)handle;
    
    pthread_mutex_lock(&base_obj->lock);
    // Only accept messages if still running
    if (base_obj->running){
        enqueue(base_obj->queue, id);
        pthread_cond_signal(&base_obj->cv);
    }
    pthread_mutex_unlock(&base_obj->lock);
}

void bt_destroy (void* handle) {
    base_thread_t* base_obj = (base_thread_t*) handle;
    
    if(!base_obj){
        printf("Could not cast handle\n");
        return;
    }
    
    pthread_mutex_lock(&base_obj->lock);
    base_obj->running = 0;
    pthread_cond_broadcast(&base_obj->cv);  
    pthread_mutex_unlock(&base_obj->lock);
    
    pthread_join(base_obj->tid, NULL);

    pthread_mutex_lock(&base_obj->lock);
    free_list(base_obj->queue);
    pthread_mutex_unlock(&base_obj->lock);
    

    pthread_cond_destroy(&base_obj->cv);
    pthread_mutex_destroy(&base_obj->lock);
    
    free(base_obj);

}

void* worker_loop(void* arg){
    base_thread_t* base_obj = (base_thread_t*) arg;
    
    while (1) {
        pthread_mutex_lock(&base_obj->lock);
        
        // Wait while queue is empty AND still running
        while (base_obj->running && is_empty(base_obj->queue)) {
            pthread_cond_wait(&base_obj->cv, &base_obj->lock);
        }
        
        // Check for shutdown - exit if not running and queue is empty
        if (!base_obj->running && is_empty(base_obj->queue)) {
            pthread_mutex_unlock(&base_obj->lock);
            break;
        }
        
        // Process message if available
        uint32_t msg_id = 0;
        int has_message = 0;
        
        if (!is_empty(base_obj->queue)) {
            msg_id = dequeue(&base_obj->queue);
            has_message = 1;
        }
        
        // Check for BREAK signal (message 0) - initiate shutdown
        if (has_message && msg_id == 0) {
            printf("BASE_THREAD: Received BREAK signal (0), initiating shutdown...\n");
            base_obj->running = 0;  // Set shutdown flag
            pthread_mutex_unlock(&base_obj->lock);
            
            // Still call the callback to let the owner know about the shutdown
            if (base_obj->cb) {
                base_obj->cb(base_obj->owner, msg_id);
            }
            break;  // Exit the loop
        }
        
        // Keep references to callback and owner while still holding lock
        process_fn_t callback = base_obj->cb;
        void* owner = base_obj->owner;
        
        pthread_mutex_unlock(&base_obj->lock);
        
        // Execute callback outside of critical section
        if (has_message && callback) {
            callback(owner, msg_id);
        }
    }

    return NULL;
}

interface_t* base_get_interface_functions(){
    return &base_thread_vtable;
}