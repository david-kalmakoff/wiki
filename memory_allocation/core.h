#ifndef MEMORY_CORE_H_

#define MEMORY_CORE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef MEMORY_ASSERT
#include <assert.h>
#define MEMORY_ASSERT assert
#endif // MEMORY_ASSERT

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif // DEFAULT_ALIGNMENT

inline bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

inline uintptr_t align_forward_uintptr(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;

  MEMORY_ASSERT(is_power_of_two(align));

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

  MEMORY_ASSERT(is_power_of_two((uintptr_t)align));

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

  MEMORY_ASSERT(is_power_of_two(alignment));

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

#endif // MEMORY_CORE_H_
