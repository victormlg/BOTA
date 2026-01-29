#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <assert.h>

typedef enum {
  LEXER_ERROR,
  PARSER_ERROR
} ErrorType;

void IntepreterError(ErrorType type, const char *msg, size_t lineno);

#endif
