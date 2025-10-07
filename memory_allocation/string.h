#ifndef MEMORY_STRING_H_
#define MEMORY_STRING_H_

#include "arena.h"

typedef struct {
  Arena *arena;
  char *data;
  size_t size;
  size_t capacity;
} String;

String create_string(Arena *arena, char *data) {
  int capacity = strlen(data) * 2;
  String a = {0};
  a.arena = arena;
  a.size = 0;
  a.capacity = capacity;
  a.data = (char *)arena_alloc(arena, (sizeof *a.data) * capacity);
  MEMORY_ASSERT(a.data != NULL);
  strcpy(a.data, data);
  return a;
}

void resize_string(String *a, size_t capacity) {
  void *new_data =
      arena_resize(a->arena, a->data, (sizeof *a->data) * a->capacity,
                   (sizeof *a->data) * capacity);
  MEMORY_ASSERT(new_data != NULL);

  a->data = (char *)new_data;
  a->capacity = capacity;
}

void string_push(String *a, char *data) {
  int data_len = strlen(data);

  if (a->size + data_len + 1 > a->capacity) {
    resize_string(a, (a->capacity + data_len + 1) * 2);
  }
  strcat(a->data, data);
  a->size += data_len + 1;
}

char *string_get(String *a) { return a->data; }

void string_clear(String *a) {
  memset(a->data, 0, a->capacity);
  a->size = 0;
}
#endif // MEMORY_STRING_H_
