/*
 * =====================================================================================
 *
 *       Filename:  processHandler.c
 *
 *    Description: List of functions that manage the process of finding the
 * appropriate trail(s) for the user.
 *
 *        Version:  1.0
 *        Created:  12/13/2024 04:13:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * =====================================================================================
 */

#include "processHandler.h"
#include "fileHandler.h"
#include "inputHandler.h"
#include "throwError.h"

/*
    This function determines the prompt mode of the program
*/
void process_DeterminePrompt(const char *fileName, PromptMode userPromptMode) {

  double finishThreshold = 0;

  switch (userPromptMode) {
    /*
        If --prompt==interactive or -p=interactive was passed as a command line
       argument, the user is asked the input for the time threshold within which
       the user would like to finish the trail.
       The last number in the file which would otherwise determine the threshold
       is ignored in this case
    */
  case PROMPT_INTERACTIVE:
    finishThreshold =
        validateInput("[*] Enter the time window to finish the trail: ");
    break;

  /*
    If --prompt=disabled or -p=disabled was passed as a command line argument,
    the user is asked the input for the time threshold within which the user
    would like to finish the trail but no description about the input is printed
    to the console.
  */
  case PROMPT_DISABLED:
    finishThreshold = validateInput("");
    break;

  /*
    No command line arguments passed except --input=filename or -i=filename, the
    timing threshold is determined by parsing the last line of the file that
    determines the timing threshold within which the user would like to finish
    the trail
  */
  case PROMPT_NONE:
    finishThreshold = extract_Threshold(fileName);
    break;
  // If there are no valid command line argumentts, throw an error
  default:
    throwOnError(0, "Invalid prompt mode.");
  }

  /*
    Calculate the threshold filter for determining the suitable trails
  */
  double trailFilter = finishThreshold * 5;
  process_TrailFinder(fileName, trailFilter, finishThreshold);
}

// This function starts the process of finding the suitable trails for the user
void process_TrailFinder(const char *fileName, double trailFilter,
                         double finishThreshold) {

  int filteredTrails = 0;     // Keep count of the filtered trails
  Trail *listOfTrails = NULL; // List of all the trails

  FILE *fdescriptor = fopen(fileName, "r");
  throwOnError(fdescriptor != NULL,
               "Failed to open input file - the file doesn't exist or cannot "
               "be opened (check permissions).");

  /*
    Validate the trail data - make sure that the first number matches the actual
    amount of trails in the source input file. If the file is validated, return
    the number of trails present in the file and save it into the variable
  */
  int numberOfTrails = validate_TrailData(&fdescriptor, fileName);

  /*
    Read the trails into an array
  */
  read_Trails(fdescriptor, numberOfTrails, &listOfTrails);
  fclose(fdescriptor);

  OK("File %s read operation successful.", fileName);

  NEWLINE;

  INFO("Trails :");

  for (int i = 0; i < numberOfTrails; ++i) {
    printf("%s%s", listOfTrails[i].trailName,
           (i < numberOfTrails - 1) ? ", " : "\n\n");
  }
  /*
    Sort the trails and sort them based
    on their distance from the shortest trail to the longest one
  */
  sort_Trails(numberOfTrails, listOfTrails);

  INFO("Trail lengths : ");
  // Print the trails along with the lengths
  // At this point they are already sorted and printed in the order defined
  // above
  for (int i = 0; i < numberOfTrails; ++i) {
    printf("[%d] %s: %.2f km\n", i + 1, listOfTrails[i].trailName,
           listOfTrails[i].trailLength);
  }

  // Filter the trails that fit the users' required timing threshold
  Trail *listOfFilteredTrails =
      filter_Trails(numberOfTrails, listOfTrails, trailFilter, &filteredTrails);

  NEWLINE;
  if (filteredTrails > 0) {
    INFO("Trails that can be finished within %.2f hour(s): ", finishThreshold);
    for (int i = 0; i < filteredTrails; ++i) {
      int hours, minutes, seconds;
      /*
         Convert the amount of time it would take to finish a trail into
         hours, minutes and seconds
      */
      convertTrailThresholds(listOfFilteredTrails[i].trailLength / 5.0, &hours,
                             &minutes, &seconds);
      // If it takes less than an hour to finish the trail(s), don't print the
      // hours
      hours > 0
          ? printf("[%d] %s: %d hours, %d minutes, %d seconds to finish.\n",
                   i + 1, listOfFilteredTrails[i].trailName, hours, minutes,
                   seconds)
          // If it takes more than an hour, print the hours as well
          : printf("[%d] %s: %d minutes, %d seconds to finish.\n", i + 1,
                   listOfFilteredTrails[i].trailName, minutes, seconds);
    }
  } else {
    /*
        If there is no trail that can be finished within the user's provided
        threshold, print out the shortest trail there is as a suggestion
    */
    INFO("There is no suitable trail for your time window of %.2f hour(s).",
         finishThreshold);
    INFO("The shortest trail is : ");
    int hours, minutes, seconds;
    convertTrailThresholds(listOfTrails[0].trailLength / 5.0, &hours, &minutes,
                           &seconds);
    // Same logic as above
    hours > 0 ? printf("[%d] %s: %d hours, %d minutes, %d seconds to finish.\n",
                       1, listOfTrails[0].trailName, hours, minutes, seconds)
              : printf("[%d] %s: %d minutes, %d seconds to finish.\n", 1,
                       listOfTrails[0].trailName, minutes, seconds);
  }
  // Free the memory of the arrays
  cleanup(numberOfTrails, listOfTrails);
  cleanup(filteredTrails, listOfFilteredTrails);
}