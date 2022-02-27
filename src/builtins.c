#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h> 

#include "builtins.h"
#include "io_helpers.h"
#include "variables.h"
// ====== Command execution =====

/* Return: index of builtin or -1 if cmd doesn't match a builtin
 */
bn_ptr check_builtin(const char *cmd) {
    ssize_t cmd_num = 0;
    while (cmd_num < BUILTINS_COUNT &&
           strncmp(BUILTINS[cmd_num], cmd, MAX_STR_LEN) != 0) {
               if (cmd_num == 1 && strchr(cmd, *BUILTINS[cmd_num]) != NULL) {
                   return BUILTINS_FN[1];
               }
        cmd_num += 1;
    }
    return BUILTINS_FN[cmd_num];
}


// ===== Builtins =====

/* Prereq: tokens is a NULL terminated sequence of strings.
 * Return 0 on success and -1 on error ... but there are no errors on echo. 
 */
ssize_t bn_echo(Node* envVars, int* numOfEnvVar, char **tokens) {
    ssize_t index = 2;

    if (strncmp(tokens[index], "$", 1) == 0) {

        while (tokens[index] != NULL) {
            char *varName = strtok(tokens[index], "$");
            //printf("%lu\n", strlen(varName));
            if (EnvVarExist(envVars, *numOfEnvVar, varName, strlen(varName)) == 1) 
            {
                //printf("if block works");
                char *rtn = EnvVarReturn(envVars, *numOfEnvVar, varName, strlen(varName));
                display_message(rtn);
                display_message(" ");
            }
            index += 2;
        }
        display_message("\n"); 
    }
    else 
    {
        if (tokens[index] != NULL) {
            // TODO:
            // Implement the echo command
            display_message(tokens[index]);
            index += 1;
        }
        while (tokens[index] != NULL) {
            // TODO:
            // Implement the echo command
            display_message(tokens[index]);
            index += 1;
        }
        display_message("\n");
    }

    return 0;
}

ssize_t bn_var(Node* envVars, int* numOfEnvVar, char **tokens) {
    if (EnvVarExist(envVars, *numOfEnvVar, tokens[0], strlen(tokens[0])) == 0) //Doesn't exist
    {
        AddEnvVar(envVars, numOfEnvVar, tokens[0], strlen(tokens[0]), tokens[1], strlen(tokens[1]));
    }
    else if (EnvVarExist(envVars, *numOfEnvVar, tokens[0], strlen(tokens[0])) == 1) //Does exist
    {
        EnvVarChange(envVars, *numOfEnvVar, tokens[0], strlen(tokens[0]), tokens[1], strlen(tokens[1]));
    }
    return 0;
}

//============================MILESTONE 3 core functions=================================

//Checks whether if a variable represents a valid path
//Returns NULL if it can't be found
char* variable_path(Node* envVars, int* numOfEnvVar, char* path_name) {
    if (EnvVarExist(envVars, *numOfEnvVar, path_name, strlen(path_name)) == 1) {
        char *rtn = EnvVarReturn(envVars, *numOfEnvVar, path_name, strlen(path_name));
        if (access(rtn, F_OK) == 0) {
            return rtn;
        }
        return NULL;
    }
    return NULL;
}

ssize_t bn_cat(Node* envVars, int* numOfEnvVar, char **tokens) {
    //Check if a variable that represents path is given
    char* path = tokens[2];
    char* varPath = variable_path(envVars, numOfEnvVar, tokens[2]);
    if (varPath != NULL) {
        path = varPath;
    }

    if (access(path, F_OK) == 0) {
        FILE *f = fopen(path, "rb"); 
        char *s = malloc(64 + 1);

        while (fgets(s, 64, f) != NULL) {
            display_message(s);
        }
        fclose(f);
        display_message("\n");
        free(s);
        return 0;
    }
    else if (path == NULL){
        display_message("ERROR: No input source provided");
        display_message("\n");
        return -1;
    }
    else {
        display_message("ERROR: Cannot open file");
        display_message("\n");
        return -1;
    }
    return -1;
}

ssize_t bn_wc(Node* envVars, int* numOfEnvVar, char **tokens) {
    //Check if a variable that represents path is given
    char* path = tokens[2];
    char* varPath = variable_path(envVars, numOfEnvVar, tokens[2]);
    if (varPath != NULL) {
        path = varPath;
    }

    if (access(path, F_OK) == 0) {
        FILE *f = fopen(path, "rb");

        int numOfChars = 0;
        int numOfLines = 0;
        int numOfWords = 0;

        char space[2] = " ";
        char *newline = "\n";
        char *temp;

        int LastCharWasSpace = 0; //<- originally set as false, 0 == false
        do
        {
            // Taking input single character at a time
            char c = fgetc(f);
            temp = &c;

            // Checking for end of file
            if (feof(f)) {
                break ;
            }
            
            if (strncmp(temp, space, 1) == 0 && LastCharWasSpace == 0) {
                numOfWords++;
                numOfChars++;
                LastCharWasSpace = 1;
            }
            else if (strncmp(temp, newline, 1) == 0) {
                numOfLines++;
                numOfChars++;
                numOfWords++;
                LastCharWasSpace = 0;
            }
            else {
                numOfChars++;
                LastCharWasSpace = 0;
            }
            
 
        }  while(1);

        if (numOfLines != 0) {
            numOfLines++;
        }
        if (numOfWords != 0) {
            numOfWords++;
        }

        fclose(f);

        display_message("word count ");
        printf("%d\n", numOfWords);
        //display_message("\n");
        display_message("character count ");
        printf("%d\n", numOfChars);
        //display_message("\n");
        display_message("newline count ");
        printf("%d\n", numOfLines);
        //display_message("\n");
        return 0;
    }
    else if (path == NULL){
        display_message("ERROR: No input source provided");
        display_message("\n");
        return -1;
    }
    else {
        display_message("ERROR: Cannot open file");
        display_message("\n");
        return -1;
    }
    return 0;
}

ssize_t bn_pwd (Node* envVars, int* numOfEnvVar, char **tokens) {
    char* g = getcwd(NULL, 64);
    display_message(g);
    display_message("\n");
    return 0;
}

ssize_t bn_cd(Node* envVars, int* numOfEnvVar, char **tokens) {
    //Check if a variable that represents path is given
    if (tokens[2] == NULL) {
        chdir(getenv("HOME"));
        return 0;
    }
    char* path = tokens[2];
    char* varPath = variable_path(envVars, numOfEnvVar, tokens[2]);
    if (varPath != NULL) {
        path = varPath;
    }

    char *sq_line = "~";
    char *parent_start = "..";
    //if the file is absolute path or file under current path
    if (access(path, F_OK) == 0) {
        chdir(path);
        return 0;
    }
    //cd ~
    else if (strncmp(path, sq_line, 1) == 0){
        chdir(getenv("HOME"));
        return 0;
    }
    else if (strncmp(path, parent_start, 1) == 0) {
        //display_message("bello");
        ssize_t err = cd_helper(path);
        return err;
    }
    else {
        display_message("ERROR: Cannot open file");
        display_message("\n");
        return -1;
    }
    return 0;
}

ssize_t cd_helper(char *path) {
    chdir("..");
    char *parent = ".";
    char *index = path;
    index = (char*)(index + 2);
    while (index != NULL) {
        if (strncmp(index, parent, 1) == 0) {
            chdir("..");
            index = (char*)(index + 1);
        }
        else if (access(index, F_OK)) {
            chdir(index);
            return 0;
        }
        else {
            display_message("ERROR: Invalid path");
            display_message("\n");
            return -1;
        }
    }
    return 0;
}

//Returns the index of --rec
int get_rec_index(char** tokens) {
    int i = 0;
    char *recurse = "--rec";
    while (tokens[i] != NULL) {
        if (strncmp(tokens[i], recurse, 5) == 0 && tokens[i+2] != NULL) {
            if (access(tokens[i+2], F_OK) == 0) {
                return i;
            }
        }
        i++;
    }
    return -1;
}

//Get the depth in form of int, even if it's stored as a variable 
int get_depth(Node* envVars, int* numOfEnvVar, char **tokens) {
    int i = 0;
    char *depth = "--d";
    while (tokens[i] != NULL) {
        if (strncmp(tokens[i], depth, 3) == 0 && tokens[i+2] != NULL) {
            if (isdigit(*tokens[i+2]) != 0) {
                int rtn = strtol(tokens[i+2], NULL, 10);
                return rtn;
            }
            else if (EnvVarExist(envVars, *numOfEnvVar, tokens[i+2], strlen(tokens[i+2]))) {
                char* temp = EnvVarReturn(envVars, *numOfEnvVar, tokens[i+2], strlen(tokens[i+2]));
                int rtn = strtol(temp, NULL, 10);
                return rtn; 
            }
        }
        i++;
    }
    return -1;
}

//Get substring, variable has superiority to regular substring
char* get_subs(Node* envVars, int* numOfEnvVar, char **tokens) {
    int i = 0;
    char *substring = "--f";
    while (tokens[i] != NULL) {
        if (strncmp(tokens[i], substring, 3) == 0 && tokens[i+2] != NULL) {
            if (EnvVarExist(envVars, *numOfEnvVar, tokens[i+2], strlen(tokens[i+2]))) {
                char* rtn = EnvVarReturn(envVars, *numOfEnvVar, tokens[i+2], strlen(tokens[i+2]));
                return rtn;
            }
            return tokens[i+2];
        }
        i++;
    }
    return NULL;
}

ssize_t bn_ls(Node* envVars, int* numOfEnvVar, char **tokens) {
    //char *recurse = "--rec";
    //char *depth = "--d";

    //ls
    if (tokens[2] == NULL) {
        recur_base();
        return 0;
    }

    //Check if the format looks like ls --ref path --d depth
    else if (get_rec_index(tokens) != -1 && get_depth(envVars, numOfEnvVar, tokens) != -1) {

        //Check if the format looks like ls --ref path --d depth --f substring
        if (get_subs(envVars, numOfEnvVar, tokens) != NULL) {
            //Convert the tokens for ls to appropriate format for cd 
            int idx = get_rec_index(tokens);

            char** temp = (char**)(tokens + idx);
            char* substring = get_subs(envVars, numOfEnvVar, tokens);

            //saves the current directory
            char c[200];
            char *cwd = NULL;
            cwd = getcwd(c, 200);

            //Change the current directory according to input through cd
            bn_cd(envVars, numOfEnvVar, temp);
            DIR* d = opendir(".");

            int depth = get_depth(envVars, numOfEnvVar, tokens);
            ssize_t err = recursive_ls_wstring(d, depth, substring);

            //Change the directory back
            chdir(cwd);
            return err;
        }

        //Convert the tokens for ls to appropriate format for cd 
        int idx = get_rec_index(tokens);
        char** temp = (char**)(tokens + idx);

        //saves the current directory
        char c[200];
        char *cwd = NULL;
        cwd = getcwd(c, 200);

        //Change the current directory according to input through cd
        bn_cd(envVars, numOfEnvVar, temp);
        DIR* d = opendir(".");

        int depth = get_depth(envVars, numOfEnvVar, tokens);
        ssize_t err = recursive_ls(d, depth);

        //Change the directory back
        chdir(cwd);
        return err;
    }

    //ls --f string
    else if (get_subs(envVars, numOfEnvVar, tokens) != NULL) {
        char* substring = get_subs(envVars, numOfEnvVar, tokens);
        get_item_wstring(substring);
        return 0;
    }

    // ls --rec path or ls --d path or any other
    return -1;
}

void get_item_wstring (char* substring) {
    DIR *d;
    struct dirent *dir;
    char* dot = ".";
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, dot, 1) != 0 && strstr(dir->d_name, substring) != NULL) {
                display_message(dir->d_name);
                display_message("\n");
            }
        }
        closedir(d);
    }
    
}

void recur_base () {
    DIR *d;
    struct dirent *dir;
    char* dot = ".";
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, dot, 1) != 0) {
                display_message(dir->d_name);
                display_message("\n");
            }
        }
        closedir(d);
    }  
}

ssize_t recursive_ls(DIR* d, int depth) {
    if (depth == 0) {
        recur_base();
        return 0;
    }
    else {
        DIR *d;
        DIR *sub_d;
        struct dirent *dir;
        char* dot = ".";
        d = opendir(".");
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, dot, 1) != 0) {
                display_message(dir->d_name);
                display_message("\n");

                sub_d = opendir(dir->d_name);
                if (sub_d != NULL) {
                    chdir(dir->d_name);
                    recursive_ls(sub_d, depth - 1);
                    chdir("..");
                }
            }
        }
        return 0; 
    }
    return 0;
}

ssize_t recursive_ls_wstring(DIR* d, int depth, char* substring) {
    if (depth == 0) {
        get_item_wstring(substring);
        return 0;
    }
    else {
        DIR *d;
        DIR *sub_d;
        struct dirent *dir;
        char* dot = ".";
        d = opendir(".");
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, dot, 1) != 0 && strstr(dir->d_name, substring) != NULL) {
                display_message(dir->d_name);
                display_message("\n");

                sub_d = opendir(dir->d_name);
                if (sub_d != NULL) {
                    chdir(dir->d_name);
                    recursive_ls_wstring(sub_d, depth - 1, substring);
                    chdir("..");
                }
            }
        }
        return 0; 
    }
    return 0;
}
