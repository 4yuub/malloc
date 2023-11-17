#include "malloc.h"

void free(void *ptr)
{
  if (!ptr)
    return;

  t_block block = search_block(ptr);

  if (!block) // not allocated by malloc
    return;

  block->free = true;
}
