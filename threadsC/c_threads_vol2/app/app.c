#include <stdlib.h>
#include <dispatcher_process.h> 


int main (int argc, char* argv[]) {
    printf("Hello world \n");

    void* dispatcher_instance = create_dispatcher();

    if (dispatcher_instance == NULL) {
        printf("Could not get dispatcher instance ! \n");
        return 1;
    } 

    uint32_t* supported_messages;
    size_t count_msgs;
    
    interface_t* dispatcher_functions = get_dispatcher_functions(dispatcher_instance);
    dispatcher_functions->get_supported_msg(dispatcher_instance, &supported_messages, &count_msgs);

    int found_flag = 0;
    char* possible_char = NULL;
    uint32_t id = 0;
    possible_char = '\0';

    for (size_t i = 1; i < (size_t)argc; i++) {
        id = strtoul(argv[i], &possible_char, 0);
        for (size_t j = 0; j < count_msgs; j++) {
            if (id == supported_messages[j]) {
                found_flag = 1;
            }
        }

        if (found_flag == 0 || *possible_char != '\0' || id == 0) {
            printf("Failure, id is unsupported or typo mistake \n");
            dispatcher_functions->destroy(dispatcher_instance);
            return 1;
        }
        dispatcher_functions->send_msg(dispatcher_instance, id);
    }

    dispatcher_functions->destroy(dispatcher_instance);
    printf("Exit SUCCESSFUL \n");
    return 0;
}