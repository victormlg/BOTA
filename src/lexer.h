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
  INTEGER,
  STRING,
  FLOAT,
  BOOL,
  PATH,
  STRUCT,
  ENUM,
  TRUE_VAL,
  FALSE_VAL,
  NOVAL,
  // Special chars
  LPAR, RPAR, LBRACE, RBRACE, LBRACKET, RBRACKET, QUOTE, COLON, SEMICOLON, COMMA,
  ADD, MUL, SUB, DIV, 
  ARROW, PIPE, ASSIGN, DOT,
  EQUAL, NOT_EQUAL, GREATER, LESSER, GREATER_EQUAL, LESSER_EQUAL,
  IDENTIFIER
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
