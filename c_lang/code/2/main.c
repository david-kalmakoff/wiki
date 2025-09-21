#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  char *header = "HTTP/1.1 200 OK";
  write(1, header, strlen(header));

  printf("\n\nThat output was from write(). This is from printf: %s\n", header);

  return 0;
}
