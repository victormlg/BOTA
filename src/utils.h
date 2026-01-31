#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

typedef enum {
  LEXER_ERROR,
  PARSER_ERROR
} ErrorType;

void InterpreterError(ErrorType type, const char *msg, size_t lineno);
void Info(const char *msg);

bool StringEqual(const char *a, const char *b);

#endif
