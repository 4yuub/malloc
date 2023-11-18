#include "malloc.h"

void show_alloc_mem(void)
{
    t_block block;
    size_t total_size = 0;
    printf("TINY : %p\n", malloc_state.tiny ? malloc_state.tiny->ptr : NULL);

    block = malloc_state.tiny_alloc;
    while (block)
    {
        printf("%p - %p : %zu bytes\n", block->ptr, block->ptr + block->size, block->size);
        total_size += block->size;
        block = block->next;
    }

    printf("SMALL : %p\n", malloc_state.small ? malloc_state.small->ptr : NULL);

    block = malloc_state.small_alloc;
    while (block)
    {
        printf("%p - %p : %zu bytes\n", block->ptr, block->ptr + block->size, block->size);
        total_size += block->size;
        block = block->next;
    }

    printf("LARGE : %p\n", malloc_state.large_alloc ? malloc_state.large_alloc->ptr : NULL);

    block = malloc_state.large_alloc;
    while (block)
    {
        printf("%p - %p : %zu bytes\n", block->ptr, block->ptr + block->size, block->size);
        total_size += block->size;
        block = block->next;
    }

    printf("Total : %zu bytes\n", total_size);
}