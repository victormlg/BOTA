#include <stdio.h>
#include "utils.h"
#include "lexer.h"

#define BUFFER_SIZE 1024

static int Run(ScannerContext *ctx, const char *text_buffer)
{
  while (ctx->counter < ctx->length && ctx->error == 0)
  {
    ScanNext(ctx, text_buffer);
  }
  return ctx->error;
}

static int RunPrompt()
{
  char text_buffer[BUFFER_SIZE];
  size_t length = 0;

  int c;
  int ret = 0;

  ScannerContext ctx;
  ctx.counter = 0;
  ctx.lineno = 0;
  ctx.num_tokens = 0;

  printf(">>> ");
  while (ret == 0)
  {
    c = fgetc(stdin);
    text_buffer[length++] = (char) c;

    if (c == '\n')
    {
      printf(">>> ");
      ctx.length = length;
      ret = Run(&ctx, text_buffer);
    }
  }
  return ret;
}

static int RunFile(const char *source)
{
  char text_buffer[BUFFER_SIZE];
  FILE *f = fopen(source, "r");

  if (f == NULL)
  {
    return 1;
  }

  size_t read = fread(text_buffer, sizeof(char), BUFFER_SIZE, f);
  fclose(f);

  ScannerContext ctx;
  ctx.counter = 0;
  ctx.lineno = 0;
  ctx.num_tokens = 0;
  ctx.length = read;

  return Run(&ctx, text_buffer);
}

int main(int argc, char **argv)
{
  printf("Welcome to BOTA 🌀\n");
  if (argc == 1)
  {
    return RunPrompt();
  }
  
  if (argc == 2)
  {
    return RunFile(argv[argc-1]);
  }
  printf("Error: Too many arguments\n");
  return 0;
}
