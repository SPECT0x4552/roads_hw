#include "fileHandler.h"
#include "throwError.h"

void r_p_Trails(FILE* fdescriptor) {
    char trailName[10]; 
    int trailNumber; 

    while(throwOnRead(fscanf(fdescriptor, "%s", trailName), "fscanf()")) {
        printf("%d. rada : %s\n", trailNumber, trailName); 
    }
}


void r_p_Lengths(FILE* fdescriptor) {
    char buf[BUFFERSIZE]; 
    int lineArr[LINES];
    size_t currentLine = 0; 


    while(throwOnRead(fgets(buf, sizeof(buf), fdescriptor))) {
        if(sscanf(buf, "%d", &lineArr[currentLine]) != 1) {
            throwOnError(NULL, "Bad line formatting"); 
        }
        ++currentLine; 

        for(size_t i = 0; i < currentLine; ++i) {
            printf("%d\n", lineArr[i]); 
        }
    } 
}