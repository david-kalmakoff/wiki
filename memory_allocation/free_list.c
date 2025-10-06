#include "memory.h"

/*
 * Free List
 *
 * https://www.gingerbill.org/article/2021/11/30/memory-allocation-strategies-005/
 *
 */

void free_list_free_all(FreeList *fl) {
  fl->used = 0;
  FreeListNode *first_node = (FreeListNode *)fl->data;
  first_node->block_size = fl->size;
  first_node->next = NULL;
  fl->head = first_node;
}

void free_list_init(FreeList *fl, void *data, size_t size) {
  fl->data = data;
  fl->size = size;
  free_list_free_all(fl);
}

FreeListNode *free_list_find_first(FreeList *fl, size_t size, size_t alignment,
                                   size_t *padding_,
                                   FreeListNode **prev_node_) {
  // Iterates the list and finds the first free block with enough space
  FreeListNode *node = fl->head;
  FreeListNode *prev_node = NULL;

  size_t padding = 0;

  while (node != NULL) {
    padding = calc_with_header((uintptr_t)node, (uintptr_t)alignment,
                               sizeof(FreeListAllocationHeader));
    size_t required_space = size + padding;
    if (node->block_size >= required_space) {
      break;
    }
    prev_node = node;
    node = node->next;
  }

  if (padding_)
    *padding_ = padding;
  if (prev_node_)
    *prev_node_ = prev_node;

  return node;
}

FreeListNode *free_list_find_best(FreeList *fl, size_t size, size_t alignment,
                                  size_t *padding_, FreeListNode **prev_node_) {
  // This iterates the entire list to find the best fit
  size_t smallest_diff = ~(size_t)0;

  FreeListNode *node = fl->head;
  FreeListNode *prev_node = NULL;
  FreeListNode *best_node = NULL;

  size_t padding = 0;

  while (node != NULL) {
    padding = calc_with_header((uintptr_t)node, (uintptr_t)alignment,
                               sizeof(FreeListAllocationHeader));
    size_t required_space = size + padding;
    if (node->block_size >= required_space &&
        (node->block_size - required_space < smallest_diff)) {
      best_node = node;
    }
    prev_node = node;
    node = node->next;
  }

  if (padding_)
    *padding_ = padding;
  if (prev_node_)
    *prev_node_ = prev_node;

  return node;
}

/* void *free_list_alloc(FreeList *fl, size_t size, size_t alignment) { */
/*   size_t padding = 0; */
/*   FreeListNode *prev_node = NULL; */
/*   FreeListNode *node = NULL; */
/*   size_t alignment_padding, required_space, remaining; */
/*   FreeListAllocationHeader *header_ptr; */
/**/
/*   if (size < sizeof(FreeListNode)) { */
/*     size = sizeof(FreeListNode); */
/*   } */
/*   if (alignment < 8) { */
/*     alignment = 8; */
/*   } */
/**/
/*   if (fl->policy == PlacementPolicyFindBest) { */
/*     node = free_list_find_best(fl, size, alignment, &padding, &prev_node); */
/*   } else { */
/*     node = free_list_find_first(fl, size, alignment, &padding, &prev_node);
 */
/*   } */
/*   assert(node != NULL && "Free list has no free memory"); */
/**/
/*   alignment_padding = padding - sizeof(FreeListAllocationHeader); */
/*   required_space = size + padding; */
/*   remaining = node->block_size - required_space; */
/**/
/*   if (remaining > 0) { */
/*     FreeListNode *new_node = (FreeListNode *)((char *)node + required_space);
 */
/*     new_node->block_size = rest; */
/*     free_list_node_insert(&fl->head, node, new_node); */
/*   } */
/**/
/*   free_list_node_remove(&fl->head, prev_node, node); */
/**/
/*   header_ptr = (FreeListAllocationHeader *)((char *)node +
 * alignment_padding); */
/*   header_ptr->block_size = required_space; */
/*   header_ptr->padding = alignment_padding; */
/**/
/*   fl->used += required_space; */
/**/
/*   return (void *)((char *)header_ptr + sizeof(FreeListAllocationHeader)); */
/* } */
