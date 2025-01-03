/*
 * =====================================================================================
 *
 *       Filename:  helpHandler.c
 *
 *    Description: Function for displaying the user manual.
 *
 *        Version:  1.0
 *        Created:  12/13/2024 03:13:58 PM
 *       Compiler:  gcc
 *
 *         Author:  Karl Marten Jaaska,
 *   Organization:  TalTech
 *
 * =====================================================================================
 */

#include "helpHandler.h"
#include <stdio.h>
#include <stdlib.h>

/*
       Not much to say here, this function is called when --help command line
   argument is passed alongside the executable and the manual is just printed
   out and the program is exited
*/

void displayProgramHelp(void) {
  printf("Usage: ./trails [OPTIONS]\n");
  printf("\n");
  printf("Options:\n");
  printf("  -i=<input_file>, --input=<input_file>    Specify the input file "
         "containing trail data.\n");
  printf("  -p=interactive, --prompt=interactive    Enable interactive mode "
         "for user input.\n");
  printf("  -p=disabled, --prompt=disabled          Disable prompt mode "
         "(non-interactive).\n");
  printf(
      "  -h, --help                              Display this help manual.\n");
  printf("\n");
  printf("Description:\n");
  printf("  This program processes trail data from a specified input file and "
         "performs the following:\n");
  printf("  - Validates the input file structure and trail data.\n");
  printf("  - Corrects mismatched trail counts if necessary.\n");
  printf("  - Filters trails based on a user-provided or pre-defined "
         "threshold.\n");
  printf("  - Outputs sorted trail information and filtered results.\n");
  printf("\n");
  printf("Examples:\n");
  printf("  ./trails --input=081.txt\n");
  printf("  ./trails -i=082.txt --prompt=interactive\n");
  printf("  ./trails -i=081.txt -p=disabled\n");
  printf("\n");
  exit(EXIT_SUCCESS);
}
