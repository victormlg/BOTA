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
  token->start = ctx->counter;
  token->end = ctx->counter + 1;
}

void ScanNext(ScannerContext *ctx, const char *buffer)
{
  assert(ctx->num_tokens < TOKEN_BUFFER_SIZE - 1 && "Token buffer should not be full");

  char c = buffer[ctx->counter];
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
      break;
    case '|':
      break;
    case '=':
      break;
    case '!':
      break;
    case '<':
      break;
    case '>':
      break;
    case '/':
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
        IntepreterError(LEXER_ERROR, "Unexpected character", ctx->lineno);
      }
  }
}

