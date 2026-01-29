#include <stdio.h>
#include "utils.h"

void InterpreterError(ErrorType type, const char *msg, size_t lineno)
{
  switch (type) {
    case LEXER_ERROR:
      printf("Lexer Error on line %ld: ", lineno);
      break;
    case PARSER_ERROR:
      printf("Parser Error on line %ld: ", lineno);
      break;
    default:
      break;
  }

  printf("%s\n", msg);
}

void Info(const char *msg)
{
  printf("%s\n", msg);
}
