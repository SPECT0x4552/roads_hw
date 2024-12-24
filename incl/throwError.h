#ifndef THROW_ERROR_H
#define THROW_ERROR_H

// macros for status printing or error messages or whatever
#define OK(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define ERR(msg, ...) fprintf(stderr, "[-] " msg "\n", ##__VA_ARGS__)
#define INFO(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
#define WARN(msg, ...) printf("[!] " msg "\n", ##__VA_ARGS__)

// macro for command line arguments
#define ARG(i) (argc > i ? argv[i] : NULL)

void throwOnError(int confirmation, const char *operationName, ...);

void *throwOnMallocError(size_t allocatedSize, const char *mallocError);

#endif // THROW_ERROR_H