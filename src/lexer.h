#include <stdlib.h>

#ifndef LEXER_H
#define LEXER_H

#include "utils.h"

typedef enum {
  // Reserved words
  AND = 0, 
  OR, 
  NOT,
  IF, 
  ELSE,
  THEN,
  LET,
  INTEGER_TYPE,
  STRING_TYPE,
  FLOAT_TYPE,
  BOOL_TYPE,
  PATH_TYPE,
  FUNCTION_TYPE,
  DURATION_TYPE,
  TIMESTAMP_TYPE,
  SCHEDULE_TYPE,
  STRUCT,
  ENUM,
  TRUE_VAL,
  FALSE_VAL,
  RESULT_TYPE,
  OPTION_TYPE,
  ASYNC_TYPE,
  OBJECT_TYPE,
  LIST_TYPE,
  NOVAL,
  // Special chars
  LPAR, RPAR, LBRACE, RBRACE, LBRACKET, RBRACKET, QUOTE, COLON, SEMICOLON, COMMA,
  ADD, MUL, SUB, DIV, 
  ARROW, PIPE, ASSIGN, DOT, COMPOSE,
  EQUAL, NOT_EQUAL, GREATER_THAN, LESS_THAN, GREATER_EQUAL, LESS_EQUAL,
  // values
  IDENTIFIER, INTEGER, STRING, FLOAT, PATH, DURATION, TIMESTAMP, SCHEDULE
} TokenType;

typedef struct {
   size_t start;
   size_t end;
   TokenType type;
} Token;

#define TOKEN_BUFFER_SIZE 32

typedef struct {
  size_t lineno;
  size_t counter;
  size_t length;
  size_t token_start;
  int num_tokens;
  int error;
  Token token_buffer[TOKEN_BUFFER_SIZE]; // TODO: Make it a ring buffer
} ScannerContext;

void ScanNext(ScannerContext *ctx, const char *buffer);

#endif
