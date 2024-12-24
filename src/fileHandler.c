#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileHandler.h"
#include "formattingHandler.h"
#include "throwError.h"

#define NAME_BUFFER 256
#define READ_BUFFER_INIT 512

int validate_TrailData(FILE **fdescriptor, const char *fileName) {
  rewind(*fdescriptor); // start from the beginning of the file

  int fileDeclaredCount, verifiedCount = 0;

  throwOnError(fscanf(*fdescriptor, "%d", &fileDeclaredCount) == 1,
               "Failed to retrieve trail count.");

  char validationBuffer[NAME_BUFFER];
  double trailLengthHelper;

  while (fscanf(*fdescriptor, "%255s %lf", validationBuffer,
                &trailLengthHelper) == 2) {
    throwOnError(isalpha(validationBuffer[0]) && trailLengthHelper > 0,
                 "Invalid entry in the input file - trail name must consist of "
                 "alphabetic characters and thet trail length needs to be a "
                 "positive number.");
    ++verifiedCount;
  }

  throwOnError(feof(*fdescriptor), "Error reading input file.");

  if (fileDeclaredCount == verifiedCount) {
    OK("File validation complete. Declared trail count (%d) matches the input "
       "file count (%d).",
       fileDeclaredCount, verifiedCount);
    rewind(*fdescriptor);
    return verifiedCount;
  }

  WARN("Declared trail count (%d) in the input file does not match the actual "
       "count of trails (%d).",
       fileDeclaredCount, verifiedCount);

  char tempFileName[FILENAME_MAX];
  snprintf(tempFileName, sizeof(tempFileName), "%s.tmp", fileName);

  FILE *tempFileHandler = fopen(tempFileName, "w");
  throwOnError(tempFileHandler != NULL, "Failed to create .tmp file.");

  throwOnError(fprintf(tempFileHandler, "%d\n", verifiedCount) > 0,
               "Failed to write corrected trail count.");

  rewind(*fdescriptor);
  fscanf(*fdescriptor, "%*d");

  char lineBuffer[READ_BUFFER_INIT];
  while (fgets(lineBuffer, sizeof(lineBuffer), *fdescriptor)) {
    char *trimLine = formatWhitespace(lineBuffer);
    if (*trimLine != '\0') {
      fprintf(tempFileHandler, "%s\n", trimLine);
    }
  }

  fclose(tempFileHandler);
  fclose(*fdescriptor);

  throwOnError(remove(fileName) == 0,
               "Failed to remove the invalid input file.");
  throwOnError(rename(tempFileName, fileName) == 0,
               "Failed to rename the corrected input file.");

  *fdescriptor = fopen(fileName, "r");
  throwOnError(*fdescriptor != NULL, "Failed to reopen corrected input file.");

  rewind(*fdescriptor);
  return verifiedCount;
}

double extract_Threshold(const char *fileName) {
  FILE *fdescriptor = fopen(fileName, "r");

  throwOnError(fdescriptor != NULL, "Failed to open input file.");

  double timingThreshold = 0;
  char lineBuffer[READ_BUFFER_INIT];

  while (fgets(lineBuffer, sizeof(lineBuffer), fdescriptor)) {
    char *thresholdLine = formatWhitespace(lineBuffer);
    if (*thresholdLine == '\0') {
      continue;
    }
    timingThreshold = atof(thresholdLine);
  }
  fclose(fdescriptor);

  throwOnError(timingThreshold > 0,
               "Failed to extract the timing threshold from the input file.");

  return timingThreshold;
}

void read_Trails(FILE *fdescriptor, int numberOfTrails, Trail **listOfTrails) {

  rewind(fdescriptor);
  throwOnError(fscanf(fdescriptor, "%*d") != 1, "Failed to skip trail count.");

  *listOfTrails = (Trail *)throwOnMallocError(
      numberOfTrails * sizeof(Trail), "Trail list instance init failed");

  for (int i = 0; i < numberOfTrails; ++i) {
    char trailNameHelper[NAME_BUFFER];
    double trailLengthHelper;

    throwOnError(fscanf(fdescriptor, "%255s %lf", trailNameHelper,
                        &trailLengthHelper) == 2,
                 "Failed to retrieve trail data.");

    (*listOfTrails)[i].trailName = (char *)throwOnMallocError(
        strlen(trailNameHelper) + 1, "Trail list instance name entry failed");
    strcpy((*listOfTrails)[i].trailName, trailNameHelper);
    (*listOfTrails)[i].trailLength = trailLengthHelper;
  }
}

static int compare_Trails(const void *a, const void *b) {
  const Trail *a_Trail = (const Trail *)a;
  const Trail *b_Trail = (const Trail *)b;

  if (a_Trail->trailLength < b_Trail->trailLength)
    return -1;
  if (a_Trail->trailLength > b_Trail->trailLength)
    return 1;

  return 0;
}

void sort_Trails(int numberOfTrails, Trail *listOfTrails) {
  if (!listOfTrails || numberOfTrails <= 0)
    throwOnError(0, "Invalid parameters");

  qsort(listOfTrails, numberOfTrails, sizeof(Trail), compare_Trails);
}

Trail *filter_Trails(int numberOfTrails, Trail *listOfTrails,
                     double maxTrailLength, int *numberOfFilteredTrails) {
  Trail *listOfFilteredTrails = (Trail *)throwOnMallocError(
      numberOfTrails * sizeof(Trail), "Filtered trail list init failed");
  *numberOfFilteredTrails = 0;

  for (int i = 0; i < numberOfTrails; ++i) {
    if (listOfTrails[i].trailLength <= maxTrailLength) {
      listOfFilteredTrails[*numberOfFilteredTrails].trailName =
          (char *)throwOnMallocError(strlen(listOfTrails[i].trailName) + 1,
                                     "Filtered trail instance init failed");
      strcpy(listOfFilteredTrails[*numberOfFilteredTrails].trailName,
             listOfTrails[i].trailName);
      listOfFilteredTrails[*numberOfFilteredTrails].trailLength =
          listOfTrails[i].trailLength;
      ++(*numberOfFilteredTrails);
    }
  }

  listOfFilteredTrails = (Trail *)realloc(
      listOfFilteredTrails, *numberOfFilteredTrails * sizeof(Trail));
  return listOfFilteredTrails;
}

void cleanup(int numberOfTrails, Trail *listOfTrails) {
  for (int i = 0; i < numberOfTrails; ++i) {
    free(listOfTrails[i].trailName);
  }

  free(listOfTrails);
}