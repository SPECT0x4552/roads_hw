#ifndef READ_FILE_H
#define READ_FILE_H

#define BUFFERSIZE 150
#define LINES 100

#include <stdio.h>

typedef struct {
  char *trailName;
  double trailLength;
} Trail;

int validate_TrailData(FILE **fdescriptor, const char *fileName);
double extract_Threshold(const char *fileName);
void read_Trails(FILE *fdescriptor, int numberOfTrails, Trail **listOfTrails);

void sort_Trails(int numberOfTrails, Trail *listOfTrails);

Trail *filter_Trails(int numberOfTrails, Trail *listOfTrails,
                     double maxTrailLength, int *numberOfFilteredTrails);

void cleanup(int numberOfTrails, Trail *listOfTrails);

#endif // READ_FILE_H