#ifndef READ_FILE_H
#define READ_FILE_H

#define BUFFERSIZE 150
#define LINES      100

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void readTraiLengths(FILE* fdescriptor);
void readTrails(FILE* fdescriptor);

#endif // READ_FILE_H