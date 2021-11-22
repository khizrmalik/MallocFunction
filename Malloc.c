#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define META_SIZE sizeof(struct mem_block)

void *new_malloc(size_t size)
{
    void *ptr = sbrk(0);
    void *req = sbrk(size);
    if (req == (void*) - 1)
        return NULL;
    else{
        return ptr;
    }
}

struct mem_block
{
    size_t size;
    struct mem_block *next;
    int free;
};

struct mem_block *request_spot(struct mem_block* ult, size_t size)
{
    struct mem_block *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE); 
    if (request == (void*) - 1){
        return NULL;
    }

    if (ult) {
        ult->next = block; 
    }
    block->size = size;
    block->next = NULL;
    return block;   
}

void main()
{
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int* ptr = (int*) new_malloc(n * sizeof(int));
    printf("The value is %d\n", *ptr);
}