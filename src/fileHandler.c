#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileHandler.h"
#include "throwError.h"

int validateTrailData(FILE **fdescriptor, const char *fileName) {
  rewind(*fdescriptor); // start from the beginning of the file

  int fileDeclaredCount, verifiedCount = 0;
  throwOnError(fscanf(*fdescriptor, "%d", &fileDeclaredCount) == 1,
               "Failed to to retrieve trail data.");

  char trailLine[256];
  char trailNameHelper[256];
  double trailLengthHelper;

  while (fscanf(*fdescriptor, "%s %lf", trailNameHelper, &trailLengthHelper) ==
         2) {
    ++verifiedCount;
  }

  throwOnError(feof(*fdescriptor) || ferror(*fdescriptor), "fscanf failed.");

  if (fileDeclaredCount != verifiedCount) {
    printf("[!] The number of trails declared in the file (%d) does not "
           "match the actual count of trails (%d).\n",
           fileDeclaredCount, verifiedCount);

    char tempHelperName[256];
    snprintf(tempHelperName, sizeof(tempHelperName), "%s.tmp", fileName);

    FILE *tempHelper;
    throwOnError((tempHelper = fopen(tempHelperName, "w")) != NULL,
                 "Failed to create a temporary fix file.");

    fprintf(tempHelper, "%d\n", verifiedCount);

    rewind(*fdescriptor);

    fscanf(*fdescriptor, "%*d");
    while (fgets(trailLine, sizeof(trailLine), *fdescriptor)) {

      /*
        fgets copies the contents of the original file to the temporary one but
        fgets includes a newline operator (\n) at the end of the line so it
        results in an extra blank line being added into the file with the
        corrected data what is not what we want
      */

      /*
         I will use fprintf to cut out the newline operator and replace it with
         NULL operator
      */
      char *startFormat = trailLine;
      while (*startFormat == ' ' || *startFormat == '\t')
        startFormat++;

      size_t replaceLength = strlen(startFormat);
      while (replaceLength > 0 && (startFormat[replaceLength - 1] == '\n' ||
                                   startFormat[replaceLength - 1] == '\r' ||
                                   startFormat[replaceLength - 1] == ' ' ||
                                   startFormat[replaceLength - 1] == '\t')) {
        startFormat[--replaceLength] = '\0';
      }

      if (replaceLength > 0) {
        fprintf(tempHelper, "%s\n", startFormat);
      }
    }

    fclose(tempHelper);
    fclose(*fdescriptor);

    throwOnError(remove(fileName) == 0 && rename(tempHelperName, fileName) == 0,
                 "Failed to remove invalid data file and rename the corrected "
                 "data file.");

    OK("Corrected trail count, rewrote data file : '%s'", fileName);

    *fdescriptor = fopen(fileName, "r");
    if (!fdescriptor) {
      ERR("Failed to reopen corrected data file");
      exit(EXIT_FAILURE);
    } else {
      __asm__ __volatile__("nop");
    }
    /*
      Reset the file pointer once again so that it can be interpreted from the
      beginning
    */
    rewind(*fdescriptor);
  }
  return verifiedCount;
}

void read_Trails(FILE *fdescriptor, int numberOfTrails, Trail **listOfTrails) {

  rewind(fdescriptor);

  throwOnError(fscanf(fdescriptor, "%*d") == 0, "Failed to skip trail count.");

  *listOfTrails = (Trail *)throwOnMallocError(
      numberOfTrails * sizeof(Trail), "Trail list instance init failed");

  for (int i = 0; i < numberOfTrails; ++i) {
    char trailNameHelper[100];
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

void sort_Trails(int numberOfTrails, Trail *listOfTrails) {
  for (int i = 0; i < numberOfTrails - 1; ++i) {
    for (int j = 0; j < numberOfTrails - i - 1; ++j) {
      if (listOfTrails[j].trailLength > listOfTrails[j + 1].trailLength) {

        Trail tempHelper = listOfTrails[j];
        listOfTrails[j] = listOfTrails[j + 1];
        listOfTrails[j + 1] = tempHelper;
      }
    }
  }
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