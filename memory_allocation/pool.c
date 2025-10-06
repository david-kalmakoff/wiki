#include "memory.h"
#include <stddef.h>
#include <stdint.h>

/*
 * Pool Allocator
 *
 * https://www.gingerbill.org/article/2019/02/16/memory-allocation-strategies-004/
 *
 */

void pool_free(Pool *p, void *ptr) {
  PoolFreeNode *node;

  void *start = p->buf;
  void *end = &p->buf[p->buf_len];

  if (ptr == NULL) {
    // Ignore NULL pointers
    return;
  }

  if (!(start <= ptr && ptr < end)) {
    assert(0 && "Memory is out of bounds of the buffer in this pool");
    return;
  }

  // Push free node
  node = (PoolFreeNode *)ptr;
  node->next = p->head;
  p->head = node;
}

void pool_free_all(Pool *p) {
  size_t chunk_count = p->buf_len / p->chunk_size;
  size_t i;

  // Set all chunks to be free
  for (i = 0; i < chunk_count; i++) {
    void *ptr = &p->buf[i * p->chunk_size];
    PoolFreeNode *node = (PoolFreeNode *)ptr;
    // Push free node onto the free list
    node->next = p->head;
    p->head = node;
  }
}

void pool_init(Pool *p, void *backing_buffer, size_t backing_buffer_length,
               size_t chuck_size, size_t chunk_alignment) {
  // Align backing buffer to the specific chunk alignment
  uintptr_t initial_start = (uintptr_t)backing_buffer;
  uintptr_t start =
      align_forward_uintptr(initial_start, (uintptr_t)chunk_alignment);
  backing_buffer_length -= (size_t)(start - initial_start);

  // Align chunk size up to the required chunk_alignment
  chuck_size = align_forward_size(chuck_size, chunk_alignment);

  // Assert that the parameters passed are valid
  assert(chuck_size >= sizeof(PoolFreeNode) && "Chunk size is too small");
  assert(backing_buffer_length >= chuck_size &&
         "Backing buffer length is smaller than the chunk size");

  // Store the adjusted parameters
  p->buf = (unsigned char *)backing_buffer;
  p->buf_len = backing_buffer_length;
  p->chunk_size = chuck_size;
  p->head = NULL;

  // Set up the free list for free chunks
  pool_free_all(p);
}

Pool pool_create(size_t chunk_size, size_t count) {
  size_t size = chunk_size * count;

  void *backing_buffer =
      mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  assert(backing_buffer != NULL);

  Pool p = {0};
  pool_init(&p, backing_buffer, size, chunk_size, DEFAULT_ALIGNMENT);

  return p;
}

void destroy_pool(Pool *p) { munmap(p->buf, p->buf_len); }

void *pool_alloc(Pool *p) {
  // Get latest free node
  PoolFreeNode *node = p->head;

  if (node == NULL) {
    assert(0 && "Pool allocator has no free memory");
    return NULL;
  }

  // Pop free node
  p->head = p->head->next;

  // Zero the memory by default
  return memset(node, 0, p->chunk_size);
}
