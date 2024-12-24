#include "inputHandler.h"
#include "throwError.h"

/*
  This function prompts the user for input if --prompt=interactive or
  -p=interactive is passed as a command line argument

  It checks whether the provided argument for the input is a number or not and
  keeps the user input prompt in a loop until the user provides a integer value

  An error indicating invalid input type is printed if the user enters a input
  of invalid type, e.g. a character.
*/
double validateInput(const char *userInputPrompt) {
  double inputValue;
  char inputBuffer[100];

  while (1) {
    if (userInputPrompt && *userInputPrompt != '\0') {
      printf("%s", userInputPrompt);
    }

    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
      ERR("fgets failed");
      continue;
    }

    size_t parseLength = strlen(inputBuffer);
    if (parseLength > 0 && inputBuffer[parseLength - 1] == '\n') {
      inputBuffer[parseLength - 1] = '\0';
    }

    if (sscanf(inputBuffer, "%lf", &inputValue) == 1) {
      if (inputValue > 0) {
        return inputValue;
      } else {
        ERR("The input value must be a positive integer.");
      }
    } else {
      ERR("Invalid input");
    }
  }
}

/*
  Convert the integer values of the time it would take to finish a given trail
  into standard units of time - hours, minutes and seconds

  Use double floating point precision values for more accurate results
*/

void convertTrailThresholds(double userInputHours, int *hrs, int *min,
                            int *sec) {
  *hrs = (int)userInputHours;

  double hourFraction = userInputHours - *hrs;
  double totalMinutesUntilFinish = hourFraction * 60;

  *min = (int)totalMinutesUntilFinish;
  *sec = (int)((totalMinutesUntilFinish - *min) * 60);
}