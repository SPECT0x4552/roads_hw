/*
 * =====================================================================================
 *
 *       Filename:  fileHandler.c
 *
 *    Description: Functions for handling the input data file and extracting
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

/*
  The functions in this file handle all the operations for handling the file :
  - Validating the data of the file
  - Correcting and formatting the file there is a need for it
  - Reading the file (trails and their lengths) depending on the prompt
  parameters
  - Sorting the read data of the file (sort the trails based on their lengths
  from shortest to longest)
  - Performing cleanup operations regarding the data that was derived from the
  file
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileHandler.h"
#include "formattingHandler.h"
#include "throwError.h"

#define NAME_BUFFER 256
#define READ_BUFFER_INIT 512

/*
  Validate the trail data
*/
int validate_TrailData(FILE **fdescriptor, const char *fileName) {
  rewind(*fdescriptor); // start from the beginning of the file

  int fileDeclaredCount, verifiedCount = 0;

  throwOnError(fscanf(*fdescriptor, "%d", &fileDeclaredCount) == 1,
               "Failed to retrieve trail count.");

  char validationBuffer[NAME_BUFFER];
  double trailLengthHelper;
  /*
    When a invalid trail length entry such as e.g. -4.31 is found,
    an error is thrown and the program exits.
  */
  while (fscanf(*fdescriptor, "%255s %lf", validationBuffer,
                &trailLengthHelper) == 2) {
    throwOnError(isalpha(validationBuffer[0]) && trailLengthHelper > 0,
                 "Invalid entry in the input file - trail name must consist of "
                 "alphabetic characters and thet trail length needs to be a "
                 "positive number.");
    ++verifiedCount;
  }

  throwOnError(feof(*fdescriptor), "Error reading input file.");

  /*
    If the number of trails that was declared as the first number of the line
    correlates with the count of trails in the file, just rewind the file
    pointer to the beginning and return the correct count of trails
  */
  if (fileDeclaredCount == verifiedCount) {
    OK("File validation complete. Declared trail count (%d) matches the input "
       "file count (%d).",
       fileDeclaredCount, verifiedCount);
    rewind(*fdescriptor);
    return verifiedCount;
  }

  /*
    If the trail count did not match the actual count of the trails,
    create a temporary file, write the correct count onto the file and copy all
    the trail data over to the corrected file which will get fed into the
    function that uses the corrected file for continuing program execution
  */

  WARN("Declared trail count (%d) in the input file does not match the actual "
       "count of trails (%d).",
       fileDeclaredCount, verifiedCount);

  char tempFileName[FILENAME_MAX];
  snprintf(tempFileName, sizeof(tempFileName), "%s.tmp", fileName);

  FILE *tempFileHandler = fopen(tempFileName, "w");
  throwOnError(tempFileHandler != NULL, "Failed to create .tmp file.");

  /*
    The following functions basically copy all the data over to the corrected
    file that was created as a .tmp
  */
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

  // Close both files because of removal and renaming operations that follow
  fclose(tempFileHandler);
  fclose(*fdescriptor);

  throwOnError(remove(fileName) == 0,
               "Failed to remove the invalid input file.");
  throwOnError(rename(tempFileName, fileName) == 0,
               "Failed to rename the corrected input file.");
  /*
    Because the file descriptor is passed as a double pointer, open the
    file again so that the following functions can use the file handle
  */
  *fdescriptor = fopen(fileName, "r");
  throwOnError(*fdescriptor != NULL, "Failed to reopen corrected input file.");

  rewind(*fdescriptor); // Rewind the file pointer to the beginning of the file
                        // for other functions that will use the file such as
                        // read_Trails

  return verifiedCount;
}

/*
  This function extracts the timing threshold from the last line of the file if
  only --input=fileName is passed as a command line argument
*/
double extract_Threshold(const char *fileName) {
  FILE *fdescriptor = fopen(fileName, "r");

  throwOnError(fdescriptor != NULL, "Failed to open input file.");

  double timingThreshold = 0;
  char lineBuffer[READ_BUFFER_INIT];

  // Loop until EOF
  while (fgets(lineBuffer, sizeof(lineBuffer), fdescriptor)) {
    char *thresholdLine = formatWhitespace(lineBuffer);
    if (*thresholdLine == '\0') {
      continue;
    }
    /*
      Convert the last line that determines the timing threshold into a
      double precision floating point number
    */
    timingThreshold = atof(thresholdLine);
  }
  fclose(fdescriptor);

  throwOnError(timingThreshold > 0,
               "Failed to extract the timing threshold from the input file.");

  return timingThreshold;
}

/*
  Read the trails and store them in an array of structs that have two
  properties:
  - The trail name
  - The length of the trail
*/
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
    /*
      I don't really care about typecasting into a smaller data type
      because all I am checking is that the malloc should not return a NULL or
      invalid pointer, so it does not really matter in this case
    */
    (*listOfTrails)[i].trailName = (char *)throwOnMallocError(
        strlen(trailNameHelper) + 1, "Trail list instance name entry failed");
    strcpy((*listOfTrails)[i].trailName, trailNameHelper);
    (*listOfTrails)[i].trailLength = trailLengthHelper;
  }
}

/*
  Quicksort callback
*/
static int compare_Trails(const void *a, const void *b) {
  const Trail *a_Trail = (const Trail *)a;
  const Trail *b_Trail = (const Trail *)b;

  if (a_Trail->trailLength < b_Trail->trailLength)
    return -1;
  if (a_Trail->trailLength > b_Trail->trailLength)
    return 1;

  return 0;
}

// Quicksort sorting the trails based on their length
void sort_Trails(int numberOfTrails, Trail *listOfTrails) {
  if (!listOfTrails || numberOfTrails <= 0)
    throwOnError(0, "Invalid parameters");

  qsort(listOfTrails, numberOfTrails, sizeof(Trail), compare_Trails);
}
/*
  Filter the trails based on the threshold that was provided by the user or
  read out from the file
*/
Trail *filter_Trails(int numberOfTrails, Trail *listOfTrails,
                     double maxTrailLength, int *numberOfFilteredTrails) {
  Trail *listOfFilteredTrails = (Trail *)throwOnMallocError(
      numberOfTrails * sizeof(Trail), "Filtered trail list init failed");
  *numberOfFilteredTrails = 0;

  for (int i = 0; i < numberOfTrails; ++i) {
    if (listOfTrails[i].trailLength <= maxTrailLength) {
      listOfFilteredTrails[*numberOfFilteredTrails].trailName =
          // Allocate space for the name entry in the trail's struct
          (char *)throwOnMallocError(strlen(listOfTrails[i].trailName) + 1,
                                     "Filtered trail instance init failed");
      strcpy(listOfFilteredTrails[*numberOfFilteredTrails].trailName,
             listOfTrails[i].trailName);
      listOfFilteredTrails[*numberOfFilteredTrails].trailLength =
          listOfTrails[i].trailLength;
      ++(*numberOfFilteredTrails);
    }
  }
  /*
    Reallocate memory depending on how many elements were added into the list of
    the trails that are compatible with the user's timing threshold
  */
  listOfFilteredTrails = (Trail *)realloc(
      listOfFilteredTrails, *numberOfFilteredTrails * sizeof(Trail));
  return listOfFilteredTrails;
}

// A cleanup function that frees the memory of malloc calls
void cleanup(int numberOfTrails, Trail *listOfTrails) {
  for (int i = 0; i < numberOfTrails; ++i) {
    free(listOfTrails[i].trailName);
  }

  free(listOfTrails);
}