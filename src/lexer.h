#include <stdlib.h>

#ifndef LEXER_H
#define LEXER_H

#include "utils.h"

typedef enum {
  // Special chars
  LPAR, RPAR, LBRACE, RBRACE, LBRACKET, RBRACKET, QUOTE, COLON, SEMICOLON, COMMA,
  ADD, MUL, SUB, DIV, 
  ARROW, PIPE, ASSIGN, 
  EQUAL, NOT_EQUAL, GREATER, LESSER, GREATER_EQUAL, LESSER_EQUAL, 

  // Reserved words
  AND, OR, NOT,
  IF, ELSE, THEN,
  LET,
  INTEGER, STRING, FLOAT, BOOL, PATH,
  RESULT, OPTION, ASYNC,
  STRUCT, ENUM

} TokenType;

typedef struct {
   size_t start;
   size_t end;
   TokenType type;
} Token;

#define TOKEN_BUFFER_SIZE 8

typedef struct {
  size_t lineno;
  size_t counter;
  size_t length;
  size_t token_start;
  int num_tokens;
  int error;
  Token token_buffer[TOKEN_BUFFER_SIZE];
} ScannerContext;

void ScanNext(ScannerContext *ctx, const char *buffer);

#endif
