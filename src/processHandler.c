#include "processHandler.h"
#include "fileHandler.h"
#include "inputHandler.h"
#include "throwError.h"

void process_DeterminePrompt(const char *fileName, PromptMode userPromptMode) {

  double finishThreshold = 0;

  switch (userPromptMode) {
  case PROMPT_INTERACTIVE:
    finishThreshold =
        validateInput("[*] Enter the time window to finish the trail: ");
    break;

  case PROMPT_DISABLED:
    finishThreshold = validateInput("");
    break;
  case PROMPT_NONE:
    finishThreshold = extract_Threshold(fileName);
    break;

  default:
    throwOnError(0, "Invalid prompt mode.");
  }

  double trailFilter = finishThreshold * 5;
  process_TrailFinder(fileName, trailFilter, finishThreshold);
}

void process_TrailFinder(const char *fileName, double trailFilter,
                         double finishThreshold) {

  int filteredTrails = 0;
  Trail *listOfTrails = NULL;

  FILE *fdescriptor = fopen(fileName, "r");
  throwOnError(fdescriptor != NULL,
               "Failed to open input file - the file doesn't exist or cannot "
               "be opened (check permissions).");

  int numberOfTrails = validate_TrailData(&fdescriptor, fileName);

  read_Trails(fdescriptor, numberOfTrails, &listOfTrails);
  fclose(fdescriptor);

  OK("File %s read operation successful.", fileName);

  NEWLINE;

  INFO("Trails :");

  for (int i = 0; i < numberOfTrails; ++i) {
    printf("%s%s", listOfTrails[i].trailName,
           (i < numberOfTrails - 1) ? ", " : "\n\n");
  }

  sort_Trails(numberOfTrails, listOfTrails);

  INFO("Trail lengths : ");

  for (int i = 0; i < numberOfTrails; ++i) {
    printf("[%d] %s: %.2f km\n", i + 1, listOfTrails[i].trailName,
           listOfTrails[i].trailLength);
  }

  Trail *listOfFilteredTrails =
      filter_Trails(numberOfTrails, listOfTrails, trailFilter, &filteredTrails);

  NEWLINE;
  if (filteredTrails > 0) {
    INFO("Trails that can be finished within %.2f hour(s): ", finishThreshold);
    for (int i = 0; i < filteredTrails; ++i) {
      int hours, minutes, seconds;
      convertTrailThresholds(listOfFilteredTrails[i].trailLength / 5.0, &hours,
                             &minutes, &seconds);
      hours > 0
          ? printf("[%d] %s: %d hours, %d minutes, %d seconds to finish.\n",
                   i + 1, listOfFilteredTrails[i].trailName, hours, minutes,
                   seconds)
          : printf("[%d] %s: %d minutes, %d seconds to finish.\n", i + 1,
                   listOfFilteredTrails[i].trailName, minutes, seconds);
    }
  } else {
    INFO("There is no suitable trail for your time window of %.2f hour(s).",
         finishThreshold);
    INFO("The shortest trail is : ");
    int hours, minutes, seconds;
    convertTrailThresholds(listOfTrails[0].trailLength / 5.0, &hours, &minutes,
                           &seconds);
    hours > 0
        ? printf("[%d] %s: %d hours, %d minutes, %d seconds to finish.\n", 1,
                 listOfFilteredTrails[i].trailName, hours, minutes, seconds)
        : printf("[%d] %s: %d minutes, %d seconds to finish.\n", 1,
                 listOfFilteredTrails[i].trailName, minutes, seconds);
  }

  cleanup(numberOfTrails, listOfTrails);
  cleanup(filteredTrails, listOfFilteredTrails);
}