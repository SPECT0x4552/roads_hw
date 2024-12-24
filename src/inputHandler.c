#include "inputHandler.h"
#include "throwError.h"

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

void convertTrailThresholds(double userInputHours, int *hrs, int *min,
                            int *sec) {
  *hrs = (int)userInputHours;

  double hourFraction = userInputHours - *hrs;
  double totalMinutesUntilFinish = hourFraction * 60;

  *min = (int)totalMinutesUntilFinish;
  *sec = (int)((totalMinutesUntilFinish - *min) * 60);
}