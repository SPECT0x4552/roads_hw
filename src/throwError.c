#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "throwError.h"

/*
  This function is mostly used as a wrapper function and the confirmation
  parameter is usually the result of a comparison that was passed as an argument

  e.g. after opening a file a file descriptor comparison could be passed as an
  argument for the confirmation parameter - a comparison if the file descriptor
  is a null pointer or not
*/
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
/*
  This function has the same methodology as throwOnError, but this is
  implemented for malloc() calls to check for null pointers or mallocs that
  failed for some reason
*/
void *throwOnMallocError(size_t allocatedSize, const char *mallocError) {
  void *ptr = malloc(allocatedSize);
  if (ptr == NULL) {
    ERR("Memory allocation failed : %s", mallocError);
    exit(EXIT_FAILURE);
  }

  return ptr;
}