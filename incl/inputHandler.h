#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <stdio.h>
#include <string.h>

double validateInput(const char *userInput);
void convertTrailThresholds(double userInputHours, int *hrs, int *min,
                            int *sec);

#endif // INPUT_HANDLER_H