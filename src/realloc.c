#include "malloc.h"

void *realloc(void *ptr, size_t size)
{
  if (!ptr || size == 0) {
    free(ptr);
    return malloc(size);
  }

  t_block block = search_block(ptr);
  if (!block) {
    return NULL; // behavior is undefined
  }

  if (block->size >= size)
    return ptr;

  void *new_ptr = malloc(size);
  if (!new_ptr)
    return NULL;

  memcpy(new_ptr, ptr, block->size);
  free(ptr);
  return new_ptr;
}