/*
 * =====================================================================================
 *
 *       Filename:  fileHandler.h
 *
 *    Description: Header file for the source file that embeds functions for handling the input data file and extracting
 * data from it.
 *
 *        Version:  1.0
 *        Created:  12/12/2024 05:50:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * =====================================================================================
 */

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