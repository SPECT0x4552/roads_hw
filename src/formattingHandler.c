#include "formattingHandler.h"

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