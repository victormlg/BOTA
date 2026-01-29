#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

static int RunPrompt()
{
  char buffer[BUFFER_SIZE];
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
        buffer[length++] = (char) c;
        break;
    }
  }
  buffer[length] = '\0';
  printf("File content '%s'\n", buffer);

  return 0;
}

static int RunFile(const char *source)
{
  FILE *f = fopen(source, "r");

  if (f == NULL)
  {
    return 1;
  }

  // Do something

  fclose(f);

  return 0;
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
