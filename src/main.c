#include "helpHandler.h"
#include "processHandler.h"
#include "throwError.h"
#include "userArgumentHandler.h"

int main(int argc, char **argv) {
  char *inputFile = NULL;
  PromptMode userPromptMode = PROMPT_NONE;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      displayProgramHelp();
    }
  }

  parseUserArguments(argc, argv, &inputFile, &userPromptMode);
  if (!inputFile) {
    ERR("Error: Input file is required. Use -h or --help for usage "
        "information.");
    return EXIT_FAILURE;
  }
  process_DeterminePrompt(inputFile, userPromptMode);

  OK("All done, exiting.");
  return 0;
}