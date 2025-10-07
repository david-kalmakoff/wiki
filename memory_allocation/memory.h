#pragma once

#include "core.h"

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
