#include "bota.h"
#include "lexer.h"

#define BUFFER_SIZE 1024

static int Run(BOTAContext *ctx, const char *text_buffer)
{
  
  // for RunPrompt, should call BOTAParserStatementS
  // for RunFile, shouldcall BOTAParserProgram
  //
  // Instead of this loop, just run parser once and it will parse until there is no more token avalaible (it will can ScanNext)

  while (ctx->counter < ctx->length && ctx->error == 0)
  {
    ScanNext(ctx, text_buffer);
  }
  return ctx->error;
}

static int RunPrompt(BOTAContext *ctx, char *text_buffer)
{
  size_t length = 0;

  int c;
  int ret = 0;

  printf(">>> ");
  while (ret == 0)
  {
    c = fgetc(stdin);
    text_buffer[length++] = (char) c;

    if (c == '\n')
    {
      printf(">>> ");
      ctx->length = length;
      ret = Run(ctx, text_buffer);
    }
  }
  return ret;
}

static int RunFile(BOTAContext *ctx, char *text_buffer, const char *source)
{
  FILE *f = fopen(source, "r");

  if (f == NULL)
  {
    return 1;
  }

  size_t read = fread(text_buffer, sizeof(char), BUFFER_SIZE, f);
  fclose(f);
  ctx->length = read;

  return Run(ctx, text_buffer);
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

  char text_buffer[BUFFER_SIZE]; //TODO: empty when full, no need to malloc
  BOTAContext ctx;

  BOTAContextInit(&ctx);

  int ret = 0;
  if (argc == 1)
  {
    ret = RunPrompt(&ctx, text_buffer);
    goto cleanup;
  }
  
  if (argc == 2)
  {
    ret = RunFile(&ctx, text_buffer, argv[argc-1]);
    goto cleanup;
  }
  ret = 1;
  printf("Error: Too many arguments\n");

cleanup:
  BOTAContextDestroy(&ctx);
  return ret;
}
