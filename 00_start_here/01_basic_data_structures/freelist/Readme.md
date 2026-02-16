# Free List 

## Requirement

We need a data structure wrapper to manage a static data buffer in-place. 

This data structure should be able to :

### P0
1. Bind to any byte-aligned memory block of any size.
2. Have a fixed block size i.e. allocations and frees are all equal sized.
3. Operate entirely with malloc and free API i.e. details are abstracted.

### P1
1. Have the block size be configurable.
2. Provide thread-safety.

## Design

A freelist fits the bill. 

A freelist is a data structure that consists of a linked list of blocks in 
contiguous memory. 

Each block serves as a node with only 1 data entry, the address to the next node.

This data member [address of the next node] is placed at the begining of every
memory block. 

When malloc is called, the freelist gives the first free node to the caller. If 
no free node exists, then a NULL pointer is returned back.

When free is called, the returned memory is placed at the begining of the list, 
with the address of the previous head in the next address data member.

| [uintptr_t] [ n-bytes] | [uintptr_t] [ n-bytes] | [uintptr_t] [ n-bytes] | 

As we can observe, nodes might not be returned in order, this will creata out-of-order nodes in freelist, this is negated by having the next address for fast access, hence the need of next address although we are operating in contiguous memory. 


### Steps

1. Create a data structure to represent a freelist.
   ```
   typedef struct {
        uintptr_t head;
        size_t sz;
        size_t blk_sz;
   } freelist_t;
   ```
2. Fn to initialize the free list data block with empty blocks.
3. Fn to malloc and free