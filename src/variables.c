#include "variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_helpers.h"

struct Node* AddNode(struct Node* p_ls, int* p_size)
{
    int size = *p_size;
    if (size == 0)
    {
        *p_size += 1;
        return p_ls;
    }

    struct Node* curNode = p_ls;
    int i = 0;
    for (i = 0; i < size - 1; ++i)
    {
        curNode = curNode->next;
    }
    curNode->next = (struct Node*)malloc(sizeof(struct Node));
    *p_size += 1;
    return curNode->next;
}

void AddEnvVar(struct Node* p_ls, int* p_size, char* p_name, int p_nameSize, char* p_val, int p_valSize)
{
    
    struct Node* newNode = AddNode(p_ls, p_size);
    char* addedName = malloc((p_nameSize+1) * sizeof(char));
    char* addedVal = malloc((p_valSize+1) * sizeof(char));
    strncpy(addedName, p_name, p_nameSize+1);
    strncpy(addedVal, p_val, p_valSize+1);
    newNode->name = addedName;
    newNode->val = addedVal;
    //struct Node* node = p_ls;
    /*
    int i = 0;
    for (i = 0; i < *p_size; ++i)
    {
        
        display_message("node name is ");
        display_message(node->name);
        display_message("\n");
        display_message("node value is ");
        display_message(node->val);
        display_message("\n");
        node = node->next;
    }
    */
    
}

int EnvVarExist(struct Node* p_ls, int p_size, char* p_name, int p_nameSize)
{
    struct Node* node = p_ls;
    int idx = 0;
    for (idx = 0; idx < p_size; ++idx)
    {
        if(strncmp(p_name, node->name, p_nameSize) == 0) 
        {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void EnvVarChange(struct Node* p_ls, int p_size, char* p_name, int p_nameSize, char* p_val, int p_valSize)
{
    struct Node* node = p_ls;
    int idx = 0;
    for (idx = 0; idx < p_size; ++idx)
    {
        if(strncmp(p_name, node->name, p_nameSize) == 0) 
        {
            free(node->val);
            char* addedVal = malloc((p_valSize+1) * sizeof(char));
            strncpy(addedVal, p_val, p_valSize+1);
            node->val = addedVal;
        }
        node = node->next;
    }
}

char * EnvVarReturn(struct Node* p_ls, int p_size, char* p_name, int p_nameSize)
{
    struct Node* node = p_ls;
    int idx = 0;
    for (idx = 0; idx < p_size; ++idx)
    {
        if(strncmp(p_name, node->name, p_nameSize) == 0) 
        {
            //printf("Returns");
            return node->val;
        }
        node = node->next;
    }
    //printf("Returns null");
    return NULL;
}