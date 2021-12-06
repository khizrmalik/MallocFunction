#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define HEAP_CAP 8192 //Number of bytes allocated to the heap at a time

size_t total = HEAP_CAP;
size_t tot = 0;
size_t count = 1;

struct meta_data{ //Data structure for meta data
    size_t size;
    struct meta_data *next;
    struct meta_data *prev;
    int free;
};

void *heapHead = NULL;
#define META_SIZE sizeof(struct meta_data) //Size of meta data in bytes

void printFreeList(struct meta_data *block) {

    tot = HEAP_CAP - total;
    tot -= (count*META_SIZE);
    printf("Total Memory: %zu bytes\n", tot);
    while (block) {
        if (block->free == 1){
            printf("%p-%ld | ", block, block->size);
        }
        block = block->next;
    }
    printf("\n");
}


struct meta_data *find_free_chunk(struct meta_data **last, size_t size){
    struct meta_data *temp = heapHead;
    while(temp && !(temp->free && temp->size >= size))
    {
        *last = temp;
        temp = temp->next;
    }
    return temp;
} // Used to find a free, available chunk in the heap to allocate to

struct meta_data *req_chunk(struct meta_data* last, size_t size){

    struct meta_data *block;
    block = sbrk(0);
    void *request = sbrk(HEAP_CAP);

    // if(!heapHeadThere){
    //     struct meta_data *heapHead;
    //     heapHead = sbrk(HEAP_CAP);
    //     heapHead->size = (HEAP_CAP - META_SIZE);
    //     heapHead->free = 1;
    //     heapHeadThere = true;
    // }

    if (last)
    {
        last->next = block;
    }
    block->size = size;
    block->prev = last;
    block->next = NULL;
    block->free = 0;
    return block;
} // Uses sbrk to request a certain amount of bytes

void *new_malloc(size_t size)
{
    size_t ogSize = size;
    for (int i = 1; i < 13; i++)
    {
        if (size <= pow(2, i) && size > pow(2, i - 1))
        {
            size = pow(2, i);
        }
    }

    struct meta_data *block;
    if (size <= 0){
        return NULL;
    }

    if (!heapHead) {
        block = req_chunk(NULL, (HEAP_CAP - META_SIZE));
        if (!block){
            return NULL;
        }
        heapHead = block;
    }

    else
    {
        struct meta_data *last = heapHead;
        block = find_free_chunk(&last, size);
        if (!block){
            block = req_chunk(last, size);
            if(!block){
                return NULL;
            }
        }
        else
        {
            block->free = 0;
            block->size = ogSize;
        }
    }
    total -= (size + META_SIZE);
    printf("Allocating at: %p \n", block);
    struct meta_data* blck = heapHead;
    printFreeList(blck);
    count++;
    return(block+1);
}

//struct meta_data *get_ptr(void *ptr){
//
//    struct meta_data *ptr = heapHead;
//    while(ptr! = NULL)
//    {
//        if(ptr = addr)
//        ptr = ptr->next
//
//    }
//    return(struct meta_data*)ptr - 1;
//}

struct meta_data *get_ptr(void *ptr){
    return(struct meta_data*)ptr;
}

void new_free(void *ptr)
{
    if (!ptr) {
        return;
    }
    struct meta_data* block_ptr = get_ptr(ptr);

    block_ptr->free = 1;
    total += block_ptr->size;
    struct meta_data* block = heapHead;
    printFreeList(block);
    //TODO: If freed then change prev pointer and next pointer of current block
    // 
}

int main()
{

    while(true)
    {
        char c[50];
        int i;
        char *pt;
        printf("\n");
        scanf("%s", c);
        printf("\n");

        char tmp[50];
        strcpy(tmp, c + 1);

        if (c[0] == 'A' || c[0] == 'a')
        {
            float *ptr;
            i = atoi(tmp);
            ptr = (float*)new_malloc(0.25*i*(sizeof(float)));
        }

        else if (c[0] == 'F' || c[0] == 'f')
        {
            new_free((void*)strtol(tmp, NULL, 0));
        }

        else if (c[0] == 'q')
        {
            return 0;
        }
    }

}