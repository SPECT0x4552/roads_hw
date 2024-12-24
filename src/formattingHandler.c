#include "formattingHandler.h"

/*
  A formating utility for handling the replacement of the correct count of
  trails if there is a need to correct the count of trails in the original file

  This removes the trailing newline that gets added by default by the reading
  operations when validating the file. It's important to remove the newline that
  gets added because the functions that will read the input source file will
  othrewise fail to parse the count of trails - because of that the rest of the
  program will fail as well.
*/

char *formatWhitespace(char *formatStr) {
  char *endFormat;

  while (*formatStr == ' ' || *formatStr == '\t' || *formatStr == '\n' ||
         *formatStr == '\r') {
    ++formatStr;
  }

  if (*formatStr == '\0') {
    return formatStr;
  }

  endFormat = formatStr + strlen(formatStr) - 1;

  while (endFormat > formatStr && (*endFormat == ' ' || *endFormat == '\t' ||
                                   *endFormat == '\n' || *endFormat == '\r')) {
    *endFormat = '\0';
    --endFormat;
  }

  return formatStr;
}