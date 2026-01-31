#include "lexer.h"
#include "bota.h"

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
  Token *token = &ctx->token_buffer[ctx->num_tokens++];
  token->type = type;
  token->start = ctx->token_start;
  token->end = ctx->counter;
}

static bool Match(BOTAContext *ctx, const char *buffer, char expected)
{
  if ((ctx->counter >= ctx->length) || (buffer[ctx->counter] != expected))
  {
    return false;
  }
  ctx->counter++;
  return true;
}

static void PushStringToken(BOTAContext *ctx, const char *buffer)
{
  while (buffer[ctx->counter++] != '"')
  {
    if (buffer[ctx->counter] == '\n')
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
  Token *token = &ctx->token_buffer[ctx->num_tokens-1];
  token->start +=1;
  token->end -=1;
}

static void PushNumberToken(BOTAContext *ctx, const char *buffer)
{
  while (IsDigit(buffer[ctx->counter]))
  {
    ctx->counter++;
  }

  if (buffer[ctx->counter] == '.' && IsDigit(buffer[ctx->counter+1]))
  {
    ctx->counter++;
    while (IsDigit(buffer[ctx->counter]))
    {
      ctx->counter++;
    }
    PushToken(ctx, TOKEN_FLOAT);
  }
  else {
    PushToken(ctx, TOKEN_INTEGER);
  }
}

static void PushIdentifierToken(BOTAContext *ctx, const char *buffer)
{
  while (IsAlphaNumeric(buffer[ctx->counter]))
  {
    ctx->counter++;
  }
  
  size_t token_length = ctx->counter - ctx->token_start;
  char token_string[token_length + 1];
  memcpy(token_string, buffer + ctx->token_start, token_length);
  token_string[token_length] = '\0';

  for (int i = 0; i < TOKEN_NOVAL; i++)
  {
    if (StringEqual(identifiers[i], token_string))
    {
      PushToken(ctx, i);
      return;
    }
  }

  PushToken(ctx, TOKEN_IDENTIFIER);
}

static void PushPathToken(BOTAContext *ctx, const char *buffer)
{
  do {
    while (IsPOSIXPathname(buffer[ctx->counter]))
    {
      ctx->counter++;
    }
  } while (buffer[ctx->counter] == '/');

  PushToken(ctx, TOKEN_PATH);
}


void ScanNext(BOTAContext *ctx, const char *buffer)
{
  assert(ctx->num_tokens < TOKEN_BUFFER_SIZE - 1 && "Token buffer should not be full");

  char c = buffer[ctx->counter];
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
      PushStringToken(ctx, buffer);
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
      while (ctx->counter < ctx->length && buffer[ctx->counter] != '\n')
      {
        ctx->counter++;
      }
      break;
    case ',':
      PushToken(ctx, TOKEN_COMMA);
      break;

    // Ambiguous
    case '-':
      PushToken(ctx, Match(ctx, buffer, '>') ? TOKEN_ARROW : TOKEN_SUB);
      break;
    case '|':
      if (Match(ctx, buffer, '>'))
      {
        PushToken(ctx, TOKEN_PIPE);
      }
      else {
        InterpreterError(LEXER_ERROR, "Invalid character sequence", ctx->lineno);
        ctx->error = 2;
      }
      break;
    case '=':
      PushToken(ctx, Match(ctx, buffer, '=') ? TOKEN_EQUAL : TOKEN_ASSIGN);
      break;
    case '!':
      if (Match(ctx, buffer, '='))
      {
        PushToken(ctx, TOKEN_NOT_EQUAL);
      }
      else {
        InterpreterError(LEXER_ERROR, "Invalid character sequence", ctx->lineno);
        ctx->error = 2;
      }
      break;
    case '<':
      PushToken(ctx, Match(ctx, buffer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS_THAN);
      break;
    case '>':
      if (Match(ctx, buffer, '='))
      {
        PushToken(ctx, TOKEN_GREATER_EQUAL);
      }
      else if (Match(ctx, buffer, '>'))
      {
        PushToken(ctx, TOKEN_COMPOSE);
      }
      else {
        PushToken(ctx, TOKEN_GREATER_THAN);
      }
      break;
    case '/':
      PushPathToken(ctx, buffer);
      break;
    case '.':
      if (Match(ctx, buffer, '/'))
      {
        PushPathToken(ctx, buffer);
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
        PushNumberToken(ctx, buffer);
      }
      else if (IsAlpha(c))
      {
        PushIdentifierToken(ctx, buffer);
      }
      else {
        InterpreterError(LEXER_ERROR, "Unexpected character", ctx->lineno);
        ctx->error = 1;
      }
  }
}

