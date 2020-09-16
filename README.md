# Allocator_of_memory
Allocator of memory on C++

The class allocator which can select data in heap, with size that you say. I try to use minimum of variables, so you can see there many operations with pointers and access to them.
Memory is divided on blocks, every block contains HEADER and memory for your variable.
HEADER consist of: 
1 byte - is bool, and show is this block free.
2 bytes - is size of whole block.
2 bytes - is size of previous block.
3 bytes - empty, they actually for leveling,you can use them on your own.
SIZE_OF_MEMORY it is how many bytes reserved for your needs. Default 1024 bytes or 1 KB.
Size of HEADER you can change for your needs.
All blocks are leveling by 4. You can change it or take it off.

Functions:
void* mem_alloc(size_t size) – create block of given size and return pointer to the memory you can use. If block has more than enough memory divine block by 2. If there is no block with enough memory return nullptr.
 
Code

Result

void mem_free(void* addr) – mark this block as free. If left or right blocks free so, then unites them.
 code
Result
As you can see there was 5 blocks but after free all memory them unite in one.
	
void* mem_realloc(void* addr, size_t size) – change size of chosen block, can make it smaller or bigger(if neighboring blocks access to do it). Old memory moves to new block. If there not enough memory for moving of old block returns nullptr. If change of size successful returns pointer to the new block with old data. 
Code
Result
