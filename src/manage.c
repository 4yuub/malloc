#include "malloc.h"
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

t_malloc_state malloc_state = {0};

void increase_local_mem_size()
{
  const size_t page_size = getpagesize();
  const size_t local_mem_size = malloc_state.local_mem_size;
  // allocate new memory
  void *new_mem = mmap(NULL, local_mem_size ? local_mem_size * 2 : page_size,
                       PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  if (new_mem == MAP_FAILED)
  {
    THROW(ERR_ALLOCATION_FAILED);
    return;
  }
  if (malloc_state.local_mem != NULL)
  {
    // copy old memory to new memory
    memcpy(new_mem, malloc_state.local_mem, malloc_state.local_mem_size);

    // unmap old memory
    munmap(malloc_state.local_mem, malloc_state.local_mem_size);
  }

  // assign new memory
  malloc_state.local_mem = new_mem;
  malloc_state.local_mem_size *= 2;
}

void increase_block_size(t_block block)
{
  const size_t block_size = block->size;
  // allocate new memory
  void *new_mem = mmap(NULL, block_size * 2,
                       PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  if (new_mem == MAP_FAILED)
  {
    THROW(ERR_ALLOCATION_FAILED);
    return;
  }
  if (block->ptr != NULL)
  {
    // copy old memory to new memory
    memcpy(new_mem, block->ptr, block->size);

    // unmap old memory
    munmap(block->ptr, block->size);
  }

  // assign new memory
  block->ptr = new_mem;
  block->size *= 2;
}

t_block create_block(size_t size, t_block prev, t_block next, bool assign_mem)
{
  t_block block;

  while (malloc_state.local_mem_used_size + sizeof(struct s_block) > malloc_state.local_mem_size)
    increase_local_mem_size();

  // assign block
  block = (t_block)(malloc_state.local_mem + malloc_state.local_mem_used_size);
  malloc_state.local_mem_used_size += sizeof(struct s_block);

  block->size = size;
  block->prev = prev;
  block->next = next;
  block->free = true;
  block->ptr = NULL; // will be assigned later
  if (assign_mem)
  {
    block->ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (block->ptr == MAP_FAILED)
    {
      THROW(ERR_ALLOCATION_FAILED);
      return NULL;
    }
  }
  return block;
}

void free_block(t_block block)
{
  if (block && block->ptr != NULL)
  {
    munmap(block->ptr, block->size);
    block->ptr = NULL;
    block->free = true;
  }
}

void clean_up()
{
  // unmap all allocated memory
  free_block(malloc_state.tiny);
  free_block(malloc_state.small);
  free_block(malloc_state.large_alloc);
  if (malloc_state.local_mem != NULL)
    munmap(malloc_state.local_mem, malloc_state.local_mem_size);
}

void init()
{
  const size_t page_size = getpagesize();
  atexit(clean_up);
  // initialize malloc_state
  malloc_state.local_mem_size = 0;
  malloc_state.local_mem_used_size = 0;
  malloc_state.local_mem = NULL;
  malloc_state.tiny = create_block(page_size * 20, NULL, NULL, true);
  malloc_state.small = create_block(page_size * 40, NULL, NULL, true);
  malloc_state.tiny_size = page_size * 20;
  malloc_state.small_size = page_size * 40;
  malloc_state.tiny_used_size = 0;
  malloc_state.small_used_size = 0;
  malloc_state.tiny_alloc = NULL;
  malloc_state.small_alloc = NULL;
  malloc_state.large_alloc = NULL;
}