#include <stdio.h>
#include <string.h>
#ifdef REAL_MALLOC
#include <stdlib.h>
#else
#include "malloc.h"
#endif

int main(void)
{
  // tiny allocations
  printf("tiny allocations:\n");
  char *str = malloc(10);
  printf("- sould return an address:\n");
  printf("malloc(10): %p => %s\n", str, str != NULL ? "OK" : "KO");
  char *str2 = malloc(10);
  printf("- should return different address:\n");
  printf("malloc(10): %p => %s\n", str2, str2 != NULL && str2 != str ? "OK" : "KO");

  for (int i = 0; i < 9; i++)
    str[i] = 'a';
  str[9] = '\0';

  printf("- should be set to 'aaaaaaaaa\\0':\n");
  printf("str: %s => %s\n", str, strcmp(str, "aaaaaaaaa") == 0 ? "OK" : "KO");

  // small allocations
  printf("\nsmall allocations:\n");
  char *str3 = malloc(2000);
  printf("- sould return an address:\n");
  printf("malloc(2000): %p => %s\n", str3, str3 != NULL ? "OK" : "KO");
  char *str4 = malloc(2000);
  printf("- should return different address:\n");
  printf("malloc(2000): %p => %s\n", str4, str4 != NULL && str4 != str3 ? "OK" : "KO");



  for (int i = 0; i < 9; i++)
    str3[i] = 'a';
  str3[9] = '\0';

  printf("- should be set to 'aaaaaaaaa\\0':\n");
  printf("str3: %s => %s\n", str3, strcmp(str3, "aaaaaaaaa") == 0 ? "OK" : "KO");

  // large allocations
  printf("\nlarge allocations:\n");
  char *str5 = malloc(100000);
  printf("- sould return an address:\n");
  printf("malloc(100000): %p => %s\n", str5, str5 != NULL ? "OK" : "KO");
  char *str6 = malloc(100000);
  printf("- should return different address:\n");
  printf("malloc(100000): %p => %s\n", str6, str6 != NULL && str6 != str5 ? "OK" : "KO");

  for (int i = 0; i < 9; i++)
    str5[i] = 'a';

  str5[9] = '\0';

  printf("- should be set to 'aaaaaaaaa\\0':\n");
  printf("str5: %s => %s\n", str5, strcmp(str5, "aaaaaaaaa") == 0 ? "OK" : "KO");

  // realloc
  printf("\nrealloc:\n");
  char *str7 = realloc(str5, 9995);
  printf("- should return be the same address:\n");
  printf("(note) this may fail even if realloc is correct\n");
  printf("realloc(str5, 9995): %p => %s\n", str7, str7 == str5 ? "OK" : "KO");

  char *str8 = realloc(str, 100);
  printf("- should return a different address:\n");
  printf("realloc(str, 100): %p => %s\n", str8, str8 != str ? "OK" : "KO");

  // free
  printf("\nfree:\n");
  free(str2);
  char *str9 = malloc(10);
  printf("- should return the same address as str:\n");
  printf("(note) this may fail even if free is correct\n");
  printf("malloc(10): %p => %s\n", str9, str9 == str ? "OK" : "KO");

  char *str10 = malloc(10);
  printf("- should return the same address as str2:\n");
  printf("(note) this may fail even if free is correct\n");
  printf("malloc(10): %p => %s\n", str10, str10 == str2 ? "OK" : "KO");

  // case of malloc failure
  printf("\ncase of malloc failure:\n");
  char *str11 = malloc(-1);
  printf("- should return NULL:\n");
  printf("malloc(-1): %p => %s\n", str11, str11 == NULL ? "OK" : "KO");

#ifndef REAL_MALLOC
  // show_alloc_mem
  printf("\nshow_alloc_mem:\n");
  show_alloc_mem();
#endif
}
