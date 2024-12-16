#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include "throwError.h"
#include "fileHandler.h"

int main(int argc, char **argv) {
    throwOnError(argc, "User input"); 

    int tripLength;
    FILE* fd;

    printf("Enter the preferred length of your trip : "); 
    
     
    throwOnError(scanf("%d", &tripLength), "scanf()"); 

    throwOnError(fd = open("../resources/081.txt", "r"), "open()"); 

    r_p_Trails(fd); // read and print the trail names
    r_p_Lengths(fd);  // read and print the trail lengths


    throwOnError(close(fd), "close()"); 

    

    return 0; 
}