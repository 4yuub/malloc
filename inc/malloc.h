#ifndef MALLOC_H_INCLUDED
# define MALLOC_H_INCLUDED
# include <stddef.h>

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *free(void *ptr);
void show_alloc_mem(void);

#endif