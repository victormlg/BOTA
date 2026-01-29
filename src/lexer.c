#include "lexer.h"

static bool IsDigit(char c)
{
  return c >= '0' && c <= '9';
}

static bool IsAlpha(char c)
{
  if (c < 'a')
  {
    c += 20;
  }
  return (c >= 'a' && c <= 'z') || c == '_';
}

static bool IsAlphaNumeric(char c)
{
  return IsAlpha(c) || IsDigit(c);
}

static void PushToken(ScannerContext *ctx, TokenType type)
{
  Token *token = &ctx->token_buffer[ctx->num_tokens++];
  token->type = type;
  token->start = ctx->token_start;
  token->end = ctx->counter + 1;
}

static bool Match(ScannerContext *ctx, const char *buffer, char expected)
{
  if ((ctx->counter >= ctx->length) || (buffer[ctx->counter] != expected))
  {
    return false;
  }
  return true;
  ctx->counter++;
}


void ScanNext(ScannerContext *ctx, const char *buffer)
{
  assert(ctx->num_tokens < TOKEN_BUFFER_SIZE - 1 && "Token buffer should not be full");

  char c = buffer[ctx->counter++];
  ctx->token_start++;

  switch (c) {
    case ' ':
    case '\t':
    case '\r':
     break;

    case '\n':
      ctx->lineno++;
      break;

    case '(':
      PushToken(ctx, LPAR);
      break;
    case ')':
      PushToken(ctx, RPAR);
      break;
    case '{':
      PushToken(ctx, LBRACE);
      break;
    case '}':
      PushToken(ctx, RBRACE);
      break;
    case '[':
      PushToken(ctx, LBRACKET);
      break;
    case ']':
      PushToken(ctx, RBRACKET);
      break;
    case '"':
      // TODO
      break;
    case ':':
      PushToken(ctx, COLON);
      break;
    case ';':
      PushToken(ctx, SEMICOLON);
      break;
    case '+':
      PushToken(ctx, ADD);
      break;
    case '*':
      PushToken(ctx, MUL);
      break;
    case '#':
      // TODO
      break;
    case ',':
      PushToken(ctx, COMMA);
      break;

    // Ambiguous
    case '-':
      PushToken(ctx, Match(ctx, buffer, '>') ? ARROW : SUB);
      break;
    case '|':
      if (Match(ctx, buffer, '>'))
      {
        PushToken(ctx, PIPE);
      }
      else {
        InterpreterError(LEXER_ERROR, "Invalid character sequence", ctx->lineno);
        ctx->error = 2;
      }
      break;
    case '=':
      PushToken(ctx, Match(ctx, buffer, '=') ? EQUAL : ASSIGN);
      break;
    case '!':
      if (Match(ctx, buffer, '='))
      {
        PushToken(ctx, NOT_EQUAL);
      }
      else {
        InterpreterError(LEXER_ERROR, "Invalid character sequence", ctx->lineno);
        ctx->error = 2;
      }
      break;
    case '<':
      PushToken(ctx, Match(ctx, buffer, '=') ? LESSER_EQUAL : LESSER);
      break;
    case '>':
      PushToken(ctx, Match(ctx, buffer, '=') ? GREATER_EQUAL : GREATER);
      break;
    case '/':
      // TODO
      break;

    // Reserved Words
    default:
      if (IsDigit(c))
      {
      }
      else if (IsAlpha(c))
      {
      }
      else {
        InterpreterError(LEXER_ERROR, "Unexpected character", ctx->lineno);
        ctx->error = 1;
      }
  }
}

