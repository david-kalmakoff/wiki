#ifndef MEMORY_SLICE_H_
#define MEMORY_SLICE_H_

#include "arena.h"

typedef struct {
  Arena *arena;
  void **data;
  size_t size;
  size_t capacity;
} Slice;

Slice create_slice(Arena *arena) {
  int capacity = 8;
  Slice a = {0};
  a.arena = arena;
  a.size = 0;
  a.capacity = capacity;
  a.data = (void **)arena_alloc(arena, (sizeof *a.data) * capacity);
  MEMORY_ASSERT(a.data != NULL);
  return a;
}

void resize_slice(Slice *a, size_t capacity) {
  void *new_data =
      arena_resize(a->arena, a->data, (sizeof *a->data) * a->capacity,
                   (sizeof *a->data) * capacity);
  MEMORY_ASSERT(new_data != NULL);

  a->data = (void **)new_data;
  a->capacity = capacity;
}

void slice_push(Slice *a, void *data) {
  if (a->size + 1 > a->capacity) {
    resize_slice(a, a->capacity * 2);
  }
  a->data[a->size] = data;
  a->size++;
}

void *slice_pop(Slice *a) {
  if (a->size == 0) {
    return NULL;
  }
  void *data = a->data[a->size - 1];
  a->size--;
  return data;
}

void *slice_get(Slice *a, int index) {
  if (index < 0 || index >= a->size) {
    return NULL;
  }
  return a->data[index];
}
#endif // MEMORY_SLICE_H_
