#ifndef MALLOC_H_INCLUDED
#define MALLOC_H_INCLUDED
#include <stddef.h>
#include <stdbool.h>
#define TINY_MAX_SIZE 128
#define SMALL_MAX_SIZE 1024

void init(void);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void show_alloc_mem(void);

typedef struct s_block *t_block;

struct s_block
{
  size_t size;
  t_block next;
  t_block prev;
  bool free;
  void *ptr;
};

typedef struct s_malloc_state *t_malloc_state;

struct s_malloc_state
{
  size_t local_mem_size;
  size_t local_mem_used_size;
  void *local_mem;
  size_t tiny_size;
  size_t small_size;
  t_block tiny;
  t_block small;

  size_t tiny_used_size;
  size_t small_used_size;

  t_block tiny_alloc;
  t_block small_alloc;
  t_block large_alloc;
};

extern t_malloc_state malloc_state;
extern void *local_mem;
extern size_t local_mem_size;

#endif