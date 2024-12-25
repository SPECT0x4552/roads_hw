/*
 * =====================================================================================
 *
 *       Filename:  inputHandler.h
 *
 *    Description: Header file for inputHandler.c which defines functions for handling user input from the command line and also a function that 
 *   converts user input to time metrics.
 *
 *        Version:  1.0
 *        Created:  12/13/2024 04:34:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * =====================================================================================
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <stdio.h>
#include <string.h>

double validateInput(const char *userInput);
void convertTrailThresholds(double userInputHours, int *hrs, int *min,
                            int *sec);

#endif // INPUT_HANDLER_H