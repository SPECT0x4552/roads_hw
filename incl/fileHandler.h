#ifndef READ_FILE_H
#define READ_FILE_H

#define BUFFERSIZE 150
#define LINES 100

#include <stdio.h>

typedef struct {
  char *trailName;
  double trailLength;
} Trail;

int validateTrailData(FILE **fdescriptor, const char *fileName);

void read_Trails(FILE *fdescriptor, int numberOfTrails, Trail **listOfTrails);

void sort_Trails(int numberOfTrails, Trail *listOfTrails);

Trail *filter_Trails(int numberOfTrails, Trail *listOfTrails,
                     double maxTrailLength, int *numberOfFilteredTrails);

void cleanup(int numberOfTrails, Trail *listOfTrails);

#endif // READ_FILE_H