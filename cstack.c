#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define hstack_t int
#define UNUSED(VAR) (void)(VAR)

struct node
{
    struct node* prev;
    unsigned int size;
    void*data;
};



struct stack_entry
{
    int reserved;
    int sizeNodes;
    struct node*stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {0u, NULL};

hstack_t stack_new(void)
{
    if((g_table.entries=(stack_entry_t*)realloc( (void*)g_table.entries,(g_table.size+1)*sizeof(stack_entry_t)))!=NULL)
    {
        g_table.size=g_table.size+1;
        g_table.entries[g_table.size-1].reserved=g_table.size-1;
        g_table.entries[g_table.size-1].sizeNodes=0;
        return g_table.size-1;
    }else{
        return -1;
    }
}

void stack_free(const hstack_t hstack)
{
    int count=0;
    stack_entry_t*ptr;
    for(unsigned int i=0;i<g_table.size;i++){
        if(g_table.entries[i].reserved==hstack)
        {
            ptr= (stack_entry_t*) malloc(((g_table.size-1)*sizeof(stack_entry_t)));
            for(unsigned int i=0;i<g_table.size;i++)
            {
                if(g_table.entries[i].reserved== hstack)
                {
                    continue;
                }
                ptr[count]=g_table.entries[i];
                count++;
            }
            while(g_table.entries[i].sizeNodes!=0){
                struct node*ptr=g_table.entries[i].stack->prev;
                free(g_table.entries[i].stack);
                g_table.entries[i].stack=ptr;
                g_table.entries[i].sizeNodes=g_table.entries[i].sizeNodes-1;
            }
            free(g_table.entries);
            g_table.size=g_table.size-1;
            g_table.entries=ptr;
        }
    }
}


int stack_valid_handler(const hstack_t hstack)
{
    for(unsigned int i=0;i<g_table.size;i++)
    {
        if(g_table.entries[i].reserved==hstack)
        {
            return 0;
        }
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    for(unsigned int i=0;i<g_table.size;i++){
        if(g_table.entries[i].reserved==hstack)
        {
            return g_table.entries[i].sizeNodes;
        }
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    for(unsigned int i=0;i<g_table.size;i++){
        if((g_table.entries[i].reserved==hstack)
            &&(data_in!=NULL)
            &&(size!=0))
        {
            struct node* ptr=NULL;
            ptr=(struct node*)malloc(sizeof(struct node));
            if(ptr==NULL)
            {
                free(ptr);
                break;
            }
            g_table.entries[i].sizeNodes=g_table.entries[i].sizeNodes+1;
            ptr->data=(char*)data_in;
            ptr->size=size;
            ptr->prev=g_table.entries[i].stack;
            g_table.entries[i].stack=ptr;
        }
    }
}




unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{

    for(unsigned int i=0;i<g_table.size;i++){
        if ((g_table.entries[i].reserved==hstack)&&(data_out!=0))
        {

            if(g_table.entries[i].sizeNodes!=0)
            {
                if(g_table.entries[i].stack->size==size)
                {
                    memcpy(data_out,(char*)g_table.entries[i].stack->data,g_table.entries[i].stack->size);
                    struct node*now=g_table.entries[i].stack->prev;
                    free(g_table.entries[i].stack);
                    g_table.entries[i].stack=now;
                    g_table.entries[i].sizeNodes=g_table.entries[hstack].sizeNodes-1;
                    return size;
                }
                else
                {
                    return 0;
                }

            }
        }
    }
    return 0;


}

