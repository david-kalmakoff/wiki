#include "memory.h"

/*
 * Arena Allocator
 *
 * https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
 *
 */

// ============================================================================
// Arena
// ============================================================================

void *arena_alloc_align(Arena *a, size_t size, size_t align) {
  // Alight 'curr_offset' forward to specific alignment
  uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
  uintptr_t offset = align_forward_uintptr(curr_ptr, align);
  offset -= (uintptr_t)a->buf; // change relative offset

  // Check to see if the backing memory has space left
  if (offset + size <= a->buf_len) {
    void *ptr = &a->buf[offset];
    a->prev_offset = offset;
    a->curr_offset = offset + size;

    // Zero new memory by default
    memset(ptr, 0, size);
    return ptr;
  }
  // Return NULL if the arena is out of memory (or handle differently)
  return NULL;
}

void *arena_alloc(Arena *a, size_t size) {
  return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

void arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length) {
  a->buf = (unsigned char *)backing_buffer;
  a->buf_len = backing_buffer_length;
  a->curr_offset = 0;
  a->prev_offset = 0;
}

void arena_free(Arena *a, void *ptr) {
  // Do nothing
}

void *arena_resize_align(Arena *a, void *old_memory, size_t old_size,
                         size_t new_size, size_t align) {
  unsigned char *old_mem = (unsigned char *)old_memory;

  assert(is_power_of_two(align));

  if (old_memory == NULL || old_size == 0) {
    // Old does not exist, create new
    return arena_alloc_align(a, new_size, align);

  } else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len) {
    // Our buffer is larger than the memory

    if (a->buf + a->prev_offset == old_mem) {
      // The old memory is the last allocated thing

      a->curr_offset = a->prev_offset + new_size;

      if (new_size > old_size) {
        // Zero the new memory by default
        memset(&a->buf[a->curr_offset], 0, new_size - old_size);
      }
      return old_memory;

    } else {
      // The old memory is not the last allocated thing

      void *new_memory = arena_alloc_align(a, new_size, align);
      size_t copy_size = old_size < new_size ? old_size : new_size;

      // Copy across old memory to the new memory
      memmove(new_memory, old_memory, copy_size);
      return new_memory;
    }

  } else {
    assert(0 && "Memory is out of bounds of the buffer in this arena");
    return NULL;
  }

  return NULL;
}

void *arena_resize(Arena *a, void *old_memory, size_t old_size,
                   size_t new_size) {
  return arena_resize_align(a, old_memory, old_size, new_size,
                            DEFAULT_ALIGNMENT);
}

void arena_free_all(Arena *a) {
  a->curr_offset = 0;
  a->prev_offset = 0;
}

Arena create_arena(size_t size) {
  /* void *backing_buffer = malloc(size); */
  void *backing_buffer =
      mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  assert(backing_buffer != NULL);

  Arena a = {0};
  arena_init(&a, backing_buffer, size);

  return a;
}

void destroy_arena(Arena *a) {
  munmap(a->buf, a->buf_len);
  /* free(a->buf); */
}

// ============================================================================
// Temp Arena
// ============================================================================

TempArena temp_arena_begin(Arena *a) {
  TempArena temp = {
      .arena = a,
      .prev_offset = a->prev_offset,
      .curr_offset = a->curr_offset,
  };

  return temp;
}

void temp_arena_end(TempArena temp) {
  temp.arena->prev_offset = temp.prev_offset;
  temp.arena->curr_offset = temp.curr_offset;
}
