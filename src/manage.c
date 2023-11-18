#include "malloc.h"

t_malloc_state malloc_state = {0};

t_block search_block(void *ptr)
{
  t_block block = malloc_state.tiny_alloc;
  while (block)
  {
    if (block->ptr == ptr)
      return block;
    block = block->next;
  }

  block = malloc_state.small_alloc;
  while (block)
  {
    if (block->ptr == ptr)
      return block;
    block = block->next;
  }

  block = malloc_state.large_alloc;
  while (block)
  {
    if (block->ptr == ptr)
      return block;
    block = block->next;
  }
  return NULL;
}

void increase_local_mem_size()
{
  const size_t page_size = getpagesize();
  const size_t local_mem_size = malloc_state.local_mem_size;
  const size_t new_size = local_mem_size ? local_mem_size * 2 : page_size;
  // allocate new memory
  void *new_mem = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  if (new_mem == MAP_FAILED)
  {
    error = true;
    return;
  }

  // assign new memory
  malloc_state.local_mem = new_mem;
  malloc_state.local_mem_size = new_size;
  malloc_state.local_mem_used_size = 0;

  if (malloc_state.local_mem != NULL)
  {
    // move old memory to done with memory to be freed later
    t_block done_with_mem = malloc_state.done_with_mem;
    while (done_with_mem != NULL && done_with_mem->next != NULL)
      done_with_mem = done_with_mem->next;

    t_block new_block = create_block(local_mem_size, done_with_mem, NULL, false);
    if (error)
      return;

    if (done_with_mem == NULL)
      malloc_state.done_with_mem = new_block;

    new_block->ptr = malloc_state.local_mem;
  }
}

void increase_block_size(t_block block)
{
  const size_t block_size = block->size;
  // allocate new memory
  void *new_mem = mmap(NULL, block_size * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  if (new_mem == MAP_FAILED)
  {
    error = true;
    return;
  }
  if (block->ptr != NULL)
  {
    // move old memory to done with memory to be freed later
    t_block done_with_mem = malloc_state.done_with_mem;
    while (done_with_mem != NULL && done_with_mem->next != NULL)
      done_with_mem = done_with_mem->next;
    t_block new_block = create_block(block_size, done_with_mem, NULL, false);
    if (error)
      return;

    if (done_with_mem == NULL)
      malloc_state.done_with_mem = new_block;

    new_block->ptr = block->ptr;
  }

  // assign new memory
  block->ptr = new_mem;
  block->size *= 2;
}

t_block create_block(size_t size, t_block prev, t_block next, bool assign_mem)
{
  t_block block;

  while (malloc_state.local_mem_used_size + sizeof(struct s_block) > malloc_state.local_mem_size)
  {
    increase_local_mem_size();
    if (error)
      return NULL;
  }

  // assign block
  block = (t_block)(malloc_state.local_mem + malloc_state.local_mem_used_size);
  malloc_state.local_mem_used_size += sizeof(struct s_block);
  block->size = size;
  block->ptr = malloc_state.local_mem;
  block->prev = prev;
  if (prev != NULL)
    prev->next = block;
  block->next = next;
  if (next != NULL)
    next->prev = block;
  block->free = true;
  block->ptr = NULL; // will be assigned later
  if (assign_mem)
  {
    block->ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (block->ptr == MAP_FAILED)
    {
      error = true;
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
  free_block(malloc_state.done_with_mem);
  if (malloc_state.local_mem != NULL)
    munmap(malloc_state.local_mem, malloc_state.local_mem_size);
}

void init()
{
  static bool first = true;
  if (!first)
    return;

  const size_t page_size = getpagesize();
  // initialize malloc_state
  malloc_state.local_mem_size = 0;
  malloc_state.local_mem_used_size = 0;
  malloc_state.local_mem = NULL;
  malloc_state.done_with_mem = NULL;
  malloc_state.tiny = create_block(page_size * 20, NULL, NULL, true);
  if (error)
    return;
  malloc_state.small = create_block(page_size * 40, NULL, NULL, true);
  if (error)
  {
    free_block(malloc_state.tiny);
    return;
  }
  malloc_state.tiny_size = page_size * 20;
  malloc_state.small_size = page_size * 40;
  malloc_state.tiny_used_size = 0;
  malloc_state.small_used_size = 0;
  malloc_state.tiny_alloc = NULL;
  malloc_state.small_alloc = NULL;
  malloc_state.large_alloc = NULL;

  if (!error)
  {
    first = false;
    atexit(clean_up);
  }
}