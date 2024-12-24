#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "throwError.h"
#include "userArgumentHandler.h"

void printProgramUsage() {
  INFO("Usage : ");
  INFO("-i <input_file> / --input=<input_file>");
  INFO("-p=interactive / --prompt=interactive");
  INFO("-p=disabled / --prompt=disabled");
}

PromptMode promptModeHandler(const char *userArgument) {
  if (strcmp(userArgument, "-p=interactive") == 0 ||
      strcmp(userArgument, "--prompt=interactive") == 0) {
    return PROMPT_INTERACTIVE;
  }

  if (strcmp(userArgument, "-p=disabled") == 0 ||
      strcmp(userArgument, "--prompt=disabled") == 0) {
    return PROMPT_DISABLED;
  }

  return PROMPT_NONE;
}

void parseUserArguments(int argc, char **argv, char **inputFile,
                        PromptMode *promptMode) {
  *inputFile = NULL;
  *promptMode = PROMPT_NONE;

  for (int i = 1; i < argc; ++i) {
    if (strncmp(argv[i], "-i=", 3) == 0) {
      *inputFile = argv[i] + 3;
    } else if (strncmp(argv[i], "--input=", 8) == 0) {
      *inputFile = argv[i] + 8;
    } else if (strncmp(argv[i], "-p=", 3) == 0 ||
               strncmp(argv[i], "--prompt=", 9) == 0) {
      *promptMode = promptModeHandler(argv[i]);
    } else {
      printProgramUsage();
      throwOnError(0, "Unknown program argument: %s", argv[i]);
    }
  }

  if (!*inputFile) {
    printProgramUsage();
    throwOnError(0, "Input file is required (-i or --input).");
  }
}