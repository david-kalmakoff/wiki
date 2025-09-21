#include <stddef.h>
#include <stdio.h>
#include <string.h>

const char *DEFAULT_FILE = "index.html";

char *to_path(char *req) {
  // Input: "GET /blog HTTP/1.1..."
  // Goal: "blog/index.html"

  char *start, *end;

  for (start = req; start[0] != ' '; start++) {
    if (!start[0]) {
      return NULL;
    }
  }

  start += 2;

  for (end = start; end[0] != ' '; end++) {
    if (!end[0]) {
      return NULL;
    }
  }

  if (end[-1] != '/') {
    end[0] = '/';
    end++;
  }

  if ((size_t)end + strlen(DEFAULT_FILE) + 1 > (size_t)req + strlen(req)) {
    return NULL;
  }

  memcpy(end, DEFAULT_FILE, strlen(DEFAULT_FILE) + 1);

  return start;
}

int main() {

  char req1[] = "GET /blog HTTP/1.1\nHost: example.com";
  printf("Should be \"blog/index.html\": \"%s\"\n", to_path(req1));

  char req2[] = "GET /blog/ HTTP/1.1\nHost: example.com";
  printf("Should be \"blog/index.html\": \"%s\"\n", to_path(req2));

  char req3[] = "GET / HTTP/1.1\nHost: example.com";
  printf("Should be \"index.html\": \"%s\"\n", to_path(req3));

  char req4[] = "GET /blog ";
  printf("Should be \"(null)\": \"%s\"\n", to_path(req4));

  return 0;
}
