#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "throwError.h"

void throwOnError(int confirmation, const char *operationName, ...) {
  if (!confirmation) {
    va_list arguments;
    va_start(arguments, operationName);

    fprintf(stderr, "[-] ");
    vfprintf(stderr, operationName, arguments);
    fprintf(stderr, "\n");

    va_end(arguments);

    exit(EXIT_FAILURE);
  }
}

void *throwOnMallocError(size_t allocatedSize, const char *mallocError) {
  void *ptr = malloc(allocatedSize);
  if (ptr == NULL) {
    ERR("Memory allocation failed : %s", mallocError);
    exit(EXIT_FAILURE);
  }

  return ptr;
}