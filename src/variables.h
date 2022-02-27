#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include <unistd.h>

typedef struct Node
{
    struct Node* next;
    char* name;
    char* val;
}Node;

struct Node* AddNode(struct Node* p_ls, int* p_size);

void AddEnvVar(struct Node* p_ls, int* p_size, char* p_name, int p_nameSize, char* p_val, int p_valSize);

int EnvVarExist(struct Node* p_ls, int p_size, char* p_name, int p_nameSize);

void EnvVarChange(struct Node* p_ls, int p_size, char* p_name, int p_nameSize, char* p_val, int p_valSize);

char * EnvVarReturn(struct Node* p_ls, int p_size, char* p_name, int p_nameSize);

#endif
