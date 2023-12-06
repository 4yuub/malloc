# Custom Malloc Implementation

## Overview

This repository contains a custom implementation of the malloc memory allocation function in C. This implementation provides a dynamic memory allocation mechanism with features malloc, realloc, free, show_mem_alloc

## Features

- **malloc:** Allocates memory for a requested size, returning a pointer to the allocated block.
- **realloc:** Resizes an existing memory block allocated by malloc to a new size, preserving the original data.
- **free:** Deallocates a memory block previously allocated by malloc or realloc, returning it to the available memory pool.
- **show_mem_alloc:** Displays information about the current memory allocations, aiding in debugging and memory management.

## Example:

```c
#include <stdio.h>
#include "malloc.h"

struct Point {
  int x;
  int y;
};

int main() {
  struct Point* point;

  // Allocate memory for a structure
  point = (struct Point*) malloc(sizeof(struct Point));

  if (point == NULL) {
    printf("Error: Memory allocation failed.\n");
    return 1;
  }

  // Use the allocated memory
  point->x = 10;
  point->y = 20;

  // Print the values
  printf("Point coordinates: (%d, %d)\n", point->x, point->y);

  // Free the allocated memory
  free(point);

  return 0;
}
