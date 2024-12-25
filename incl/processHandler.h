/*
 * =====================================================================================
 *
 *       Filename:  processHandler.h
 *
 *    Description: Header file for processHandler.c that defines a list of functions that manage the process of finding the
 * appropriate trail(s) for the user.
 *
 *        Version:  1.0
 *        Created:  12/13/2024 04:13:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * =====================================================================================
 */

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