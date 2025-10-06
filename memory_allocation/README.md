# Memory Allocation Strategies

Articles from Ginger Bill (Creator of Odin language)

https://www.gingerbill.org/series/memory-allocation-strategies/

## Arena Allocator

The arena allocator acts as a lifetime for your allocations. You allocate a section of virtual memory and then create allocations within that memory space. At the end of the lifetime you can free that entire section of memory.

This allows you to not worry about freeing individual allocations. Instead you group them into lifetimes and free them at the end of their lifetimes.

With this allocation you just keep track of the current offset and allocate to that position and move the offset forward with your allocations.

You can keep track of the previous offset if you want to be able to step backwards and have temporary allocated sections of the buffer.

## Stack Allocator

The stack allocation is similar to the arena except you include a header with each allocation. This header tells you where the previous offset was. This allows you to move the offset backwards as you free certain allocations in the stack.

## Pool Allocator

The pool allocator is a sections of memory split into equally sized sections. As you use sections of memory you pull it off of the head of the free list. As you free the sections you add them back to the free list.

This allocator is useful if you have entities of the same size / type that get added and removed dynamically through the lifetime of the allocator.

## Free List Allocator

The free list allocator is similar to the pool except it accepts different sized allocations.

As you add allocations, you add a header with the size of the allocation and the free space following it.

As you remove allocations, these spaces get added to the free list. If free items are next to each other they are combined together to create a single larger space (coalesced).

## Buddy Allocator
