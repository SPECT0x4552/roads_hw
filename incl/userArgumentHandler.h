/*
 * =====================================================================================
 *
 *       Filename:  userArgumentHandler.h
 *
 *    Description: Header file for declaring a list of functions that parse the user's command line
 * arguments.
 *
 *        Version:  1.0
 *        Created:  12/21/2024 08:13:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * ========================================================================================
 */

#ifndef USER_ARGUMENT_HANDLER_H
#define USER_ARGUMENT_HANDLER_H

typedef enum { PROMPT_INTERACTIVE, PROMPT_DISABLED, PROMPT_NONE } PromptMode;

void printProgramUsage(void);
PromptMode promptModeHandler(const char *userArgument);
void parseUserArguments(int argc, char **argv, char **inputFile,
                        PromptMode *promptMode);

#endif // USER_ARGUMENT_HANDLER_H

