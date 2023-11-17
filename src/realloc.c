#include "malloc.h"

void *realloc(void *ptr, size_t size)
{
  t_block block = search_block(ptr);
  if (!block)
    return NULL; // TODO: check if this is the correct behavior

  if (block->size >= size)
    return ptr;

  void *new_ptr = malloc(size);
  if (!new_ptr)
    return NULL;

  memcpy(new_ptr, ptr, block->size);
  free(ptr);
  return new_ptr;
}