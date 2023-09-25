#include "malloc.h"
#include <stddef.h>
#include <stdbool.h>

jmp_buf jmp_env;

void *malloc(size_t size)
{
  TRY
  {
    static bool first_call = true;

    if (first_call)
    {
      init();
      first_call = false;
    }
    (void)size;
    // malloc implementation
    return NULL; // allocated memory or NULL in case of error
  }
  CATCH
  {
    // an error occured
    return NULL;
  }
}