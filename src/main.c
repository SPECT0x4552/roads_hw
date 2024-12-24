#include "helpHandler.h"
#include "processHandler.h"
#include "throwError.h"
#include "userArgumentHandler.h"

int main(int argc, char **argv) {
  char *inputFile = NULL;
  PromptMode userPromptMode = PROMPT_NONE;

  /*
    If the program was prompted with the --help command line argument, halt the
    program execution display the help manual of the program and exit.
  */
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      displayProgramHelp();
    }
  }

  /*
    Parse the user provided command line arguments depending on the arguments
    passed.
    userPromptMode is passed as a pointer that is populated depending on the
    prompt mode, the pointer is passed as a argument to determine the prompt
    that is basically the starting point to rest of the program
  */
  parseUserArguments(argc, argv, &inputFile, &userPromptMode);

  /*
    If the file handler is invalid or not retrieved, just throw an error and
    exit the program.
  */
  throwOnError(inputFile != NULL,
               "Input file is required. Use -h or --help for usage "
               "information.");

  /*
    This is the entry point of the rest of the program execution,
    prompt is determined and the processing function is called from there on
  */
  process_DeterminePrompt(inputFile, userPromptMode);

  OK("All done, exiting.");
  return 0;
}