#include <stdio.h>
#include <stdlib.h>
#include "throwError.h"

void throwOnError(int confirmation, char* operationName) {
    if(!confirmation || confirmation == NULL) {
        fprintf(stderr, "[-] %s operation failed : %d\n", &operationName, confirmation);
        exit(EXIT_FAILURE); 
    } 
    printf("[+] %s operation success : %d\n", &operationName, confirmation); 
}


int throwOnReadErr(int confirmation, char *operationName) {
    if(!confirmation || confirmation == NULL || confirmation == EOF) {
        fprintf(stderr, "[-] %s operation failed : %d\n", &operationName); 
        exit(EXIT_FAILURE); 
    }

    return 1;
}