#ifndef USER_ARGUMENT_HANDLER_H
#define USER_ARGUMENT_HANDLER_H

typedef enum { PROMPT_INTERACTIVE, PROMPT_DISABLED, PROMPT_NONE } PromptMode;

void printProgramUsage(void);
PromptMode promptModeHandler(const char *userArgument);
void parseUserArguments(int argc, char **argv, char **inputFile,
                        PromptMode *promptMode);

#endif // USER_ARGUMENT_HANDLER_H