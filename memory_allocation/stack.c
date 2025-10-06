#include "memory.h"

/*
 * Stack Allocator
 *
 * https://www.gingerbill.org/article/2019/02/15/memory-allocation-strategies-003/
 *
 */

void stack_init(Stack *s, void *backing_buffer, size_t backing_buffer_length) {
  s->buf = (unsigned char *)backing_buffer;
  s->buf_len = backing_buffer_length;
  s->prev_offset = 0;
  s->curr_offset = 0;
}

void *stack_alloc_align(Stack *s, size_t size, size_t alignment) {
  uintptr_t curr_addr, next_addr;
  size_t padding;

  StackAllocationHeader *header;

  assert(is_power_of_two(alignment));

  if (alignment > 128) {
    // The padding is 8 bits, largest alignment is 128 bytes
    alignment = 128;
  }

  curr_addr = (uintptr_t)s->buf + (uint8_t)s->curr_offset;
  padding = calc_with_header(curr_addr, (uintptr_t)alignment,
                             sizeof(StackAllocationHeader));
  if (s->curr_offset + padding + size > s->buf_len) {
    return NULL;
  }

  s->prev_offset = s->curr_offset;
  s->curr_offset += padding;

  next_addr = curr_addr + (uintptr_t)padding;
  header = (StackAllocationHeader *)(next_addr - sizeof(StackAllocationHeader));
  header->pading = (uint8_t)padding;
  header->prev_offset = s->prev_offset;

  s->curr_offset += size;

  return memset((void *)next_addr, 0, size);
}

void *stack_alloc(Stack *s, size_t size) {
  return stack_alloc_align(s, size, DEFAULT_ALIGNMENT);
}

void stack_free(Stack *s, void *ptr) {
  if (ptr != NULL) {
    uintptr_t start, end, curr_addr;
    StackAllocationHeader *header;
    size_t prev_offset;

    start = (uintptr_t)s->buf;
    end = start + (uintptr_t)s->buf_len;
    curr_addr = (uintptr_t)ptr;

    if (!(start <= curr_addr && curr_addr < end)) {
      assert(0 &&
             "Out of bounds memory address passed to stack allocator (free)");
      return;
    }

    if (curr_addr >= start + (uintptr_t)s->curr_offset) {
      // Allow double frees
      return;
    }

    header =
        (StackAllocationHeader *)(curr_addr - sizeof(StackAllocationHeader));
    prev_offset = (size_t)(curr_addr - (uintptr_t)header->pading - start);

    if (prev_offset != header->prev_offset) {
      assert(0 && "Out of order stack allocator free");
      return;
    }

    s->curr_offset = s->prev_offset;
    s->prev_offset = header->prev_offset;
  }
}

void *stack_resize_align(Stack *s, void *ptr, size_t old_size, size_t new_size,
                         size_t alignment) {
  if (ptr == NULL) {
    // NULL pointer passed in
    return stack_alloc_align(s, new_size, alignment);
  } else if (new_size == 0) {
    // Size 0 passed in
    stack_free(s, ptr);
    return NULL;
  } else {
    uintptr_t start, end, curr_addr;
    size_t min_size = old_size < new_size ? old_size : new_size;
    void *new_ptr;

    start = (uintptr_t)s->buf;
    end = start + (uintptr_t)s->buf_len;
    curr_addr = (uintptr_t)ptr;
    if (!(start <= curr_addr && curr_addr < end)) {
      assert(0 &&
             "Out of bounds memory address passed to stack allocator (free)");
      return NULL;
    }

    if (s->buf + s->prev_offset == ptr) {
      // handle resize latest allocation

      s->curr_offset = s->prev_offset + new_size;

      if (new_size > old_size) {
        // Zero the new memory by default
        memset(&s->buf[s->curr_offset], 0, new_size - old_size);
      }

      return ptr;

    } else {
      // handle non-latest allocation
      if (curr_addr >= start + (uintptr_t)s->curr_offset) {
        // Treat as a double free
        return NULL;
      }

      if (old_size == new_size) {
        return ptr;
      }

      new_ptr = stack_alloc_align(s, new_size, alignment);
      memmove(new_ptr, ptr, min_size);

      return new_ptr;
    }
  }
}

void *stack_resize(Stack *s, void *ptr, size_t old_size, size_t new_size) {
  return stack_resize_align(s, ptr, old_size, new_size, DEFAULT_ALIGNMENT);
}

void stack_free_all(Stack *s) {
  s->curr_offset = 0;
  s->prev_offset = 0;
}

Stack create_stack(size_t size) {
  void *backing_buffer =
      mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  assert(backing_buffer != NULL);

  Stack s = {0};
  stack_init(&s, backing_buffer, size);

  return s;
}

void destroy_stack(Stack *a) { munmap(a->buf, a->buf_len); }
