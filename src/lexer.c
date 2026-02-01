#include "lexer.h"
#include "bota.h"
#include <stdint.h>

const char *identifiers[] = {
  "and",
  "or",
  "not",
  "if",
  "else",
  "then",
  "let",
  "integer",
  "string",
  "float",
  "boolean",
  "path",
  "function",
  "duration",
  "timestamp",
  "schedule",
  "struct",
  "enum",
  "true",
  "false",
  "Result",
  "Option",
  "Async",
  "Object",
  "List"
};

static bool IsDigit(char c)
{
  return c >= '0' && c <= '9';
}

static bool IsAlpha(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool IsPOSIXPathname(char c)
{
  return c >= 0x20 && c != '/' && c != 0x7f;
}

static bool IsAlphaNumeric(char c)
{
  return IsAlpha(c) || IsDigit(c);
}

static void PushToken(BOTAContext *ctx, TokenType type)
{
  Token *token;
  if (ctx->num_tokens < MAX_LOOKAHEAD)
  {
    uint8_t tail = (ctx->head + ctx->num_tokens) % MAX_LOOKAHEAD;
    token = &ctx->token_buffer[tail];
    ctx->num_tokens++;

  } else {
    token = &ctx->token_buffer[ctx->head];
    ctx->head = (ctx->head + 1) % MAX_LOOKAHEAD;
  }

  token->type = type;
  token->start = ctx->token_start;
  token->end = ctx->counter;
}

static bool Match(BOTAContext *ctx, char expected)
{
  if ((ctx->counter >= ctx->length) || (ctx->text_buffer[ctx->counter] != expected))
  {
    return false;
  }
  ctx->counter++;
  return true;
}

static void PushStringToken(BOTAContext *ctx)
{
  while (ctx->text_buffer[ctx->counter++] != '"')
  {
    if (ctx->text_buffer[ctx->counter] == '\n')
    {
      ctx->lineno++;
    }

    if (ctx->counter >= ctx->length)
    {
      InterpreterError(LEXER_ERROR, "Unterminated string", ctx->lineno);
      ctx->error = 3;
      return;
    }
  }
  PushToken(ctx, TOKEN_STRING);
  // Remove quotes
  assert(ctx->num_tokens > 0 && "Tried to modify inexistent token");
  uint8_t peek_index = ctx->head % MAX_LOOKAHEAD;
  Token *token = &ctx->token_buffer[peek_index];
  token->start +=1;
  token->end -=1;
}

static void PushNumberToken(BOTAContext *ctx)
{
  while (IsDigit(ctx->text_buffer[ctx->counter]))
  {
    ctx->counter++;
  }

  if (ctx->text_buffer[ctx->counter] == '.' && IsDigit(ctx->text_buffer[ctx->counter+1]))
  {
    ctx->counter++;
    while (IsDigit(ctx->text_buffer[ctx->counter]))
    {
      ctx->counter++;
    }
    PushToken(ctx, TOKEN_FLOAT);
  }
  else {
    PushToken(ctx, TOKEN_INTEGER);
  }
}

static void PushIdentifierToken(BOTAContext *ctx)
{
  while (IsAlphaNumeric(ctx->text_buffer[ctx->counter]))
  {
    ctx->counter++;
  }
  
  size_t token_length = ctx->counter - ctx->token_start;
  for (int i = 0; i < TOKEN_NOVAL; i++)
  {
    if (memcmp(identifiers[i], &ctx->text_buffer[ctx->token_start], token_length) == 0)
    {
      PushToken(ctx, i);
      return;
    }
  }

  PushToken(ctx, TOKEN_IDENTIFIER);
}

static void PushPathToken(BOTAContext *ctx)
{
  do {
    while (IsPOSIXPathname(ctx->text_buffer[ctx->counter]))
    {
      ctx->counter++;
    }
  } while (ctx->text_buffer[ctx->counter] == '/');

  PushToken(ctx, TOKEN_PATH);
}


void ScanNext(BOTAContext *ctx)
{
  char c = ctx->text_buffer[ctx->counter];
  ctx->token_start = ctx->counter++;

  switch (c) {
    case ' ':
    case '\t':
    case '\r':
     break;

    case '\n':
      ctx->lineno++;
      break;

    case '(':
      PushToken(ctx, TOKEN_LPAR);
      break;
    case ')':
      PushToken(ctx, TOKEN_RPAR);
      break;
    case '{':
      PushToken(ctx, TOKEN_LBRACE);
      break;
    case '}':
      PushToken(ctx, TOKEN_RBRACE);
      break;
    case '[':
      PushToken(ctx, TOKEN_LBRACKET);
      break;
    case ']':
      PushToken(ctx, TOKEN_RBRACKET);
      break;
    case '"':
      PushStringToken(ctx);
      break;
    case ':':
      PushToken(ctx, TOKEN_COLON);
      break;
    case ';':
      PushToken(ctx, TOKEN_SEMICOLON);
      break;
    case '+':
      PushToken(ctx, TOKEN_ADD);
      break;
    case '*':
      PushToken(ctx, TOKEN_MUL);
      break;
    case '#':
      while (ctx->counter < ctx->length && ctx->text_buffer[ctx->counter] != '\n')
      {
        ctx->counter++;
      }
      break;
    case ',':
      PushToken(ctx, TOKEN_COMMA);
      break;

    // Ambiguous
    case '-':
      PushToken(ctx, Match(ctx, '>') ? TOKEN_ARROW : TOKEN_SUB);
      break;
    case '|':
      if (Match(ctx, '>'))
      {
        PushToken(ctx, TOKEN_PIPE);
      }
      else {
        InterpreterError(LEXER_ERROR, "Invalid character sequence", ctx->lineno);
        ctx->error = 2;
      }
      break;
    case '=':
      PushToken(ctx, Match(ctx, '=') ? TOKEN_EQUAL : TOKEN_ASSIGN);
      break;
    case '!':
      if (Match(ctx, '='))
      {
        PushToken(ctx, TOKEN_NOT_EQUAL);
      }
      else {
        InterpreterError(LEXER_ERROR, "Invalid character sequence", ctx->lineno);
        ctx->error = 2;
      }
      break;
    case '<':
      PushToken(ctx, Match(ctx, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS_THAN);
      break;
    case '>':
      if (Match(ctx, '='))
      {
        PushToken(ctx, TOKEN_GREATER_EQUAL);
      }
      else if (Match(ctx, '>'))
      {
        PushToken(ctx, TOKEN_COMPOSE);
      }
      else {
        PushToken(ctx, TOKEN_GREATER_THAN);
      }
      break;
    case '/':
      PushPathToken(ctx);
      break;
    case '.':
      if (Match(ctx, '/'))
      {
        PushPathToken(ctx);
      }
      else {
        PushToken(ctx, TOKEN_DOT);
      }
      break;

    // Reserved Words
    // TODO: Add duration, timestamp and schedule types
    default:
      if (IsDigit(c))
      {
        PushNumberToken(ctx);
      }
      else if (IsAlpha(c))
      {
        PushIdentifierToken(ctx);
      }
      else {
        InterpreterError(LEXER_ERROR, "Unexpected character", ctx->lineno);
        ctx->error = 1;
      }
  }
}

