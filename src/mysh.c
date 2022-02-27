#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "builtins.h"
#include "io_helpers.h"
#include "variables.h"

int main(int argc, char* argv[]) {
    char *prompt = "mysh$ "; // TODO Step 1, Uncomment this.

    char input_buf[MAX_STR_LEN + 1];
    input_buf[MAX_STR_LEN] = '\0';
    char *token_arr[MAX_STR_LEN] = {NULL};

    struct Node* envVars = malloc(sizeof(struct Node));
    int numberOfEnvVar = 0;

    while (1) {
        display_message(prompt);

        int ret = get_input(input_buf);
        size_t token_count = tokenize_input(input_buf, token_arr);

        // Clean exit
        if (token_count == 0) {
            continue;
        }
        //display_message(token_arr[0]);
        if (ret != -1 && (token_count == 0 || (strcmp("exit", token_arr[0]) == 0))) {
            break;
        }

        if (token_count >= 1) {
            bn_ptr builtin_fn = check_builtin(token_arr[0]);

            if (builtin_fn != NULL) {
                if (builtin_fn == bn_var) 
                {
                    tokenize_equation(input_buf, token_arr);
                    //display_message("builtin == bn_var");
                    ssize_t err = bn_var(envVars, &numberOfEnvVar, token_arr);
                    if (err == -1) 
                    {
                        display_error("ERROR: Builtin failed: ", token_arr[0]);
                    }
                }
                else 
                {
                    ssize_t err = builtin_fn(envVars, &numberOfEnvVar, token_arr);
                    if (err == -1) 
                    {
                        display_error("ERROR: Builtin failed: ", token_arr[0]);
                    }
                }
            } 
            else 
            {
                display_error("ERROR: Unrecognized command: ", token_arr[0]);
            }
            
        }
    }
    free_ll(envVars);
    return 0;
}
