#include "parser.h"
#include "config.h"

void user_input(char input[])
{
  if (strcmp(input, "END") == 0) {
    printf("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
  }
  printf("You said: ");
  printf(input);
  printf("\n");
  printf(shellprefix);
}