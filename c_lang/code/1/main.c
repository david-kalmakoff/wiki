#include <stdio.h>
#include <unistd.h>

int main() {
  write(1, "Hello World!\n", 14);

  printf("Hello World!\n");

  char *header = "HTTP/1.1 200 OK";
  printf("Header: %s\n", header);

  return 1;
}
