#include "parser.h"
#include "config.h"

void user_input(char input[])
{
  if (strcmp(input, "SHUTDOWN") == 0) {
    printf("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
  } else if (strcmp(input, "HELLO") == 0) {
    printf("Hello!");
  } else {
    printf("You said: ");
    printf(input);
  }
  printf("\n");
  printf(shellprefix);
}