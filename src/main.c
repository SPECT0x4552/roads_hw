#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileHandler.h"
#include "inputHandler.h"
#include "throwError.h"

int main(int argc, char **argv) {
  if (argc != 2)
    throwOnError(0, "Usage : %s <file_to_read.txt>", ARG(0));

  FILE *fd = fopen(ARG(1), "r");

  if (!fd) {
    ERR("Failed to open file : %s", ARG(1));
    return EXIT_FAILURE;
  }

  double finishThreshold = 0, trailFilter = 0;
  int numberOfTrails = 0, filteredTrails = 0;

  finishThreshold = validateInput(
      "[*] Enter the time window in which you'd like to finish the trail : ");

  trailFilter = finishThreshold * 5;

  numberOfTrails = validateTrailData(&fd, ARG(1));

  Trail *listOfTrails = NULL;

  read_Trails(fd, numberOfTrails, &listOfTrails);
  OK("File %s read operation successful.\n", ARG(1));

  INFO("Trails :");

  for (int i = 0; i < numberOfTrails; ++i) {
    printf("%s%s", listOfTrails[i].trailName,
           (i < numberOfTrails - 1) ? ", " : "\n\n");
  }

  sort_Trails(numberOfTrails, listOfTrails);

  INFO("Trail lengths : ");

  for (int i = 0; i < numberOfTrails; ++i) {
    printf("%s: %.2f km\n", listOfTrails[i].trailName,
           listOfTrails[i].trailLength);
  }

  Trail *listOfFilteredTrails =
      filter_Trails(numberOfTrails, listOfTrails, trailFilter, &filteredTrails);

  printf("\n");
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
    printf("[1] %s: %d minutes, %d seconds to finish.\n",
           listOfTrails[0].trailName, minutes, seconds);
  }

  fclose(fd);
  cleanup(numberOfTrails, listOfTrails);
  cleanup(filteredTrails, listOfFilteredTrails);

  OK("All done, exiting");

  return 0;
}