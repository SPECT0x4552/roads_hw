#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include "userArgumentHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void process_DeterminePrompt(const char *fileName, PromptMode userPromptMode);
void process_TrailFinder(const char *fileName, double trailFilter,
                         double finishThreshold);

#define NEWLINE printf("\n")

#endif // PROCESS_HANDLER_H