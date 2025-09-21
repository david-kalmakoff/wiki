#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *DEFAULT_FILE = "index.html";

char *to_path(char *req) {
  // Input: "GET /blog HTTP/1.1..."
  // Goal: "blog/index.html"

  char *start, *end;

  for (start = req; start[0] != ' '; start++) {
    if (!start[0]) {
      printf("unexpected end of start\n");
      return NULL;
    }
  }

  // Skip ' ' and leading '/'
  start += 2;

  for (end = start; end[0] != ' '; end++) {
    if (!end[0]) {
      printf("unexpected end of end\n");
      return NULL;
    }
  }

  if (end[-1] != '/') {
    end[0] = '/';
    end++;
  }

  if ((size_t)end + strlen(DEFAULT_FILE) + 1 > (size_t)req + strlen(req)) {
    printf("req not long enough to copy into\n");
    return NULL;
  }

  memcpy(end, DEFAULT_FILE, strlen(DEFAULT_FILE) + 1);

  return start;
}

void print_file(const char *path) {
  // Open the file in read only mode
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    printf("error openning file [%s]: %s\n", strerror(errno), path);
    return;
  }

  // Extract metadata from file
  struct stat metadata; // 144 bytes
  if (fstat(fd, &metadata) == -1) {
    printf("error reading metadata [%s]: %s\n", strerror(errno), path);
    close(fd);
    return;
  }

  // Read file to memory
  /* char buf[metadata.st_size + 1]; */
  char *buf = malloc(metadata.st_size + 1);
  if (buf == NULL) {
    printf("malloc errored\n");
    close(fd);
    return;
  }

  ssize_t bytes_read = read(fd, buf, metadata.st_size);
  if (bytes_read == -1) {
    printf("error reading bytes [%s]: %s\n", strerror(errno), path);
    free(buf);
    close(fd);
    return;
  }

  buf[bytes_read] = '\0';
  printf("\n%s contents: \n\n%s\n", path, buf);

  free(buf);
  close(fd); // close the file
}

int main() {
  char req1[] = "GET / HTTP/1.1\nHost: example.com";
  print_file(to_path(req1));

  char req2[] = "GET /blog HTTP/1.1\nHost: example.com";
  print_file(to_path(req2));

  return 0;
}
