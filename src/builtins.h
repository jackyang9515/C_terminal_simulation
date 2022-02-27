#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include <unistd.h>
#include <dirent.h>
#include "variables.h"

/* Type for builtin handling functions
 * Input: Array of tokens
 * Return: >=0 on success and -1 on error
 */
typedef ssize_t (*bn_ptr)(Node*, int*, char**);
ssize_t bn_echo(Node* envVars, int* numOfEnvVar, char **tokens); //this has to be the same argument
ssize_t bn_var(Node* envVars, int* numOfEnvVar, char **tokens);

char* variable_path(Node* envVars, int* numOfEnvVar, char* path_name);
ssize_t bn_cat(Node* envVars, int* numOfEnvVar, char **tokens);
ssize_t bn_wc(Node* envVars, int* numOfEnvVar, char **tokens);
ssize_t bn_cd(Node* envVars, int* numOfEnvVar, char **tokens);
ssize_t bn_ls(Node* envVars, int* numOfEnvVar, char **tokens);
ssize_t bn_pwd (Node* envVars, int* numOfEnvVar, char **tokens);

ssize_t cd_helper(char *path);

char* get_subs(Node* envVars, int* numOfEnvVar, char **tokens);
int get_depth(Node* envVars, int* numOfEnvVar, char **tokens);
int get_rec_index(char** tokens);
void get_item_wstring (char* substring);
ssize_t recursive_ls(DIR* d, int depth);
void recur_base();
ssize_t recursive_ls_wstring(DIR* d, int depth, char* substring);

/* Return: index of builtin or -1 if cmd doesn't match a builtin
 */
bn_ptr check_builtin(const char *cmd);


/* BUILTINS and BUILTINS_FN are parallel arrays of length BUILTINS_COUNT
 */
static const char * const BUILTINS[] = {"echo", "=", "cat", "wc", "cd", "ls", "pwd"};
static const bn_ptr BUILTINS_FN[] = {bn_echo, bn_var, bn_cat, bn_wc, bn_cd, bn_ls, bn_pwd, NULL};
static const size_t BUILTINS_COUNT = sizeof(BUILTINS) / sizeof(char *);

#endif
