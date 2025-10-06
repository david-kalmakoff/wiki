#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

// ============================================================================
// Core
// ============================================================================

inline bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

inline uintptr_t align_forward_uintptr(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;

  assert(is_power_of_two(align));

  p = ptr;
  a = (uintptr_t)align;
  // NOTE: this is faster but I don't understand it yet
  /* modulo = p & (a - 1); */
  modulo = p % a;
  if (modulo != 0) {
    p += a - modulo;
  }
  return p;
}

inline size_t align_forward_size(uintptr_t ptr, size_t align) {
  size_t p, a, modulo;

  assert(is_power_of_two((uintptr_t)align));

  p = ptr;
  a = align;
  modulo = p % a;
  if (modulo != 0) {
    p += a - modulo;
  }
  return p;
}

inline size_t calc_with_header(uintptr_t ptr, uintptr_t alignment,
                               size_t header_size) {
  uintptr_t p, a, modulo, padding, needed_space;

  assert(is_power_of_two(alignment));

  p = ptr;
  a = alignment;
  modulo = p % a;

  padding = 0;
  needed_space = 0;

  if (modulo != 0) { // Same logic as 'align_forward'
    padding = a - modulo;
  }

  needed_space = (uintptr_t)header_size;

  if (padding < needed_space) {
    needed_space -= padding;

    if (needed_space % a != 0) {
      padding += a * (1 + (needed_space / a));
    } else {
      padding += a * (needed_space / a);
    }
  }

  return (size_t)padding;
}

// ============================================================================
// Arena Allocator
// ============================================================================

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

typedef struct {
  unsigned char *buf;
  size_t buf_len;
  size_t prev_offset;
  size_t curr_offset;
} Arena;

typedef struct {
  Arena *arena;
  size_t prev_offset;
  size_t curr_offset;
} TempArena;

void *arena_alloc(Arena *a, size_t size);
void arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length);
void *arena_resize(Arena *a, void *old_memory, size_t old_size,
                   size_t new_size);
void arena_free_all(Arena *a);
Arena create_arena(size_t size);
void destroy_arena(Arena *a);
TempArena temp_arena_begin(Arena *a);
void temp_arena_end(TempArena temp);

// ============================================================================
// Stack Allocator
// ============================================================================

typedef struct {
  unsigned char *buf;
  size_t buf_len;
  size_t prev_offset;
  size_t curr_offset;
} Stack;

typedef struct {
  size_t prev_offset;
  size_t pading;
} StackAllocationHeader;

Stack create_stack(size_t size);
void destroy_stack(Stack *s);
void stack_init(Stack *s, void *backing_buffer, size_t backing_buffer_length);
void *stack_alloc_align(Stack *s, size_t size, size_t alignment);
void stack_free(Stack *s, void *ptr);
void *stack_resize_align(Stack *s, void *ptr, size_t old_size, size_t new_size,
                         size_t alignment);
void stack_free_all(Stack *s);

// ============================================================================
// Pool Allocator
// ============================================================================

typedef struct PoolFreeNode PoolFreeNode;

typedef struct PoolFreeNode {
  PoolFreeNode *next;
} PoolFreeNode;

typedef struct {
  unsigned char *buf;
  size_t buf_len;
  size_t chunk_size;

  PoolFreeNode *head;
} Pool;

void pool_free(Pool *p, void *ptr);
void pool_free_all(Pool *p);
Pool pool_create(size_t chunk_size, size_t count);
void destroy_pool(Pool *p);
void *pool_alloc(Pool *p);

// ============================================================================
// Free List Allocator
// ============================================================================

typedef struct FreeListAllocationHeader {
  size_t block_size;
  size_t padding;
} FreeListAllocationHeader;

typedef struct FreeListNode FreeListNode;

struct FreeListNode {
  FreeListNode *next;
  size_t block_size;
};

typedef enum PlacementPolicy {
  PlacementPolicyFindFirst,
  PlacementPolicyFindBest,
} PlacementPolicy;

typedef struct FreeList {
  void *data;
  size_t size;
  size_t used;

  FreeListNode *head;
  PlacementPolicy policy;
} FreeList;

// ============================================================================
// Slice
// ============================================================================

typedef struct {
  Arena *arena;
  void **data;
  size_t size;
  size_t capacity;
} Slice;

Slice create_slice(Arena *arena);
void slice_push(Slice *a, void *data);
void *slice_pop(Slice *a);
void *slice_get(Slice *a, int index);

// ============================================================================
// String
// ============================================================================

typedef struct {
  Arena *arena;
  char *data;
  size_t size;
  size_t capacity;
} String;

String create_string(Arena *arena, char *data);
void string_push(String *a, char *data);
char *string_get(String *a);
void string_clear(String *a);
