/*
 * =====================================================================================
 *
 *       Filename:  throwError.h
 *
 *    Description: Header file for declarations of error handling functions that are used as wrappers.
 *
 *        Version:  1.0
 *        Created:  12/12/2024 06:13:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * =====================================================================================
 */

#ifndef THROW_ERROR_H
#define THROW_ERROR_H

// macros for status printing or error messages or whatever
#define OK(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define ERR(msg, ...) fprintf(stderr, "[-] " msg "\n", ##__VA_ARGS__)
#define INFO(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
#define WARN(msg, ...) printf("[!] " msg "\n", ##__VA_ARGS__)


void throwOnError(int confirmation, const char *operationName, ...);

void *throwOnMallocError(size_t allocatedSize, const char *mallocError);

#endif // THROW_ERROR_H
