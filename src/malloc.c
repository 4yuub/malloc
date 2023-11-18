#include "malloc.h"

bool error;

t_block small_allocation(size_t size, bool is_tiny)
{
  t_block block = is_tiny ? malloc_state.tiny : malloc_state.small;
  size_t used_size = is_tiny ? malloc_state.tiny_used_size : malloc_state.small_used_size;
  size_t block_size = is_tiny ? malloc_state.tiny_size : malloc_state.small_size;

  t_block alloc = is_tiny ? malloc_state.tiny_alloc : malloc_state.small_alloc;

  t_block to_use = NULL;
  t_block last_alloc = NULL;
  while (alloc != NULL)
  {

    if (alloc->free && alloc->size >= size)
    {
      if (to_use == NULL || alloc->size < to_use->size)
        to_use = alloc;
    }
    last_alloc = alloc;
    if (!alloc->next)
      break;
    alloc = alloc->next;
  }
  if (to_use != NULL)
  {
    to_use->free = false;
    return to_use;
  }

  while (block->next)
    block = block->next;

  while (used_size + size > block_size)
  {
    increase_block_size(block);
    if (error)
      return NULL;
  }
  if (is_tiny)
    malloc_state.tiny_used_size = 0;
  else
    malloc_state.small_used_size = 0;

  t_block new_block = create_block(size, last_alloc, NULL, false);
  if (error)
    return NULL;
  if (last_alloc == NULL)
  {
    if (is_tiny)
      malloc_state.tiny_alloc = new_block;
    else
      malloc_state.small_alloc = new_block;
  }
  new_block->ptr = block->ptr + used_size;
  if (is_tiny)
    malloc_state.tiny_used_size += size;
  else
    malloc_state.small_used_size += size;
  new_block->free = false;
  return new_block;
}

t_block large_allocation(size_t size)
{
  t_block block = malloc_state.large_alloc;

  t_block to_use = NULL;
  t_block last_alloc = NULL;
  while (block != NULL)
  {
    if (block->free && block->size >= size)
    {
      if (to_use == NULL || block->size < to_use->size)
        to_use = block;
    }
    last_alloc = block;
    if (!block->next)
      break;
    block = block->next;
  }

  if (to_use != NULL)
  {
    to_use->free = false;
    return to_use;
  }

  t_block new_block = create_block(size, last_alloc, NULL, true);
  if (error)
    return NULL;
  if (last_alloc == NULL)
    malloc_state.large_alloc = new_block;
  new_block->free = false;
  return new_block;
}

t_block find_block(size_t size)
{
  if (size <= TINY_MAX_SIZE)
    return small_allocation(size, true);
  if (size <= SMALL_MAX_SIZE)
    return small_allocation(size, false);
  return large_allocation(size);
}

void *malloc(size_t size)
{
  error = false;

  init();
  if (error)
    return NULL;

  t_block block = find_block(size < MALLOC_MIN_SIZE ? MALLOC_MIN_SIZE : size);
  if (error)
    return NULL;
  return block->ptr;
}