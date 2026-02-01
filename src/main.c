#include "bota.h"
#include "lexer.h"
#include <stdint.h>

static uint8_t Run(BOTAContext *ctx)
{
  
  // for RunPrompt, should call BOTAParserStatementS
  // for RunFile, shouldcall BOTAParserProgram
  //
  // Instead of this loop, just run parser once and it will parse until there is no more token avalaible (it will can ScanNext)

  while (ctx->counter < ctx->length && ctx->error == 0)
  {
    ScanNext(ctx);
  }
  return ctx->error;
}

static uint8_t RunPrompt(BOTAContext *ctx)
{
  size_t length = 0;

  int c;
  uint8_t ret = 0;

  printf(">>> ");
  while (ret == 0)
  {
    c = fgetc(stdin);
    ctx->text_buffer[length++] = (char) c;

    if (c == '\n')
    {
      printf(">>> ");
      ctx->length = length;
      ret = Run(ctx);
    }
  }
  return ret;
}

static uint8_t RunFile(BOTAContext *ctx, const char *source)
{
  FILE *f = fopen(source, "r");

  if (f == NULL)
  {
    return 1;
  }

  ctx->length = fread(ctx->text_buffer, sizeof(char), TEXT_BUFFER_SIZE, f);
  fclose(f);

  return Run(ctx);
}

static void BOTAContextInit(BOTAContext *ctx)
{
  ctx->lineno = 0;
  ctx->counter = 0;
  ctx->length = 0;
  ctx->token_start = 0;

  ctx->num_tokens = 0;
  ctx->head = 0;

  ctx->error = 0;

  ctx->ast_pool = (uint8_t *) calloc(AST_POOL_SIZE, sizeof(uint8_t));
  ctx->capacity = AST_POOL_SIZE;
  ctx->current_free = 0;
}

static void BOTAContextDestroy(BOTAContext *ctx)
{
  free(ctx->ast_pool);
}

int main(int argc, char **argv)
{
  printf("Welcome to BOTA 🌀\n");

  BOTAContext ctx;

  BOTAContextInit(&ctx);

  uint8_t ret = 0;
  if (argc == 1)
  {
    ret = RunPrompt(&ctx);
    goto cleanup;
  }
  
  if (argc == 2)
  {
    ret = RunFile(&ctx, argv[argc-1]);
    goto cleanup;
  }
  ret = 1;
  printf("Error: Too many arguments\n");

cleanup:
  BOTAContextDestroy(&ctx);
  return ret;
}
