#include <stdio.h>
#include "utils.h"
#include "lexer.h"

#define BUFFER_SIZE 1024

char text_buffer[BUFFER_SIZE];

static int Run(size_t length)
{
  ScannerContext ctx;

  ctx.counter = 0;
  ctx.length = 0;
  ctx.lineno = 0;
  ctx.num_tokens = 0;

  while (ctx.counter < ctx.length)
  {
    ScanNext(&ctx, text_buffer);
  }
  return 0;
}

static int RunPrompt()
{
  size_t length = 0;

  int c;
  bool is_loop = true;

  printf(">>> ");
  while (is_loop)
  {
    c = fgetc(stdin);
    switch (c) {
      case 'q':
        is_loop = false;
        break;

      case '\n':
        printf(">>> ");
        // let through

      default:
        text_buffer[length++] = (char) c;
        break;
    }
  }
  text_buffer[length] = '\0';

  return Run(length);
}

static int RunFile(const char *source)
{
  FILE *f = fopen(source, "r");

  if (f == NULL)
  {
    return 1;
  }

  size_t read = fread(text_buffer, sizeof(char), BUFFER_SIZE, f);

  fclose(f);
  return Run(read);
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
