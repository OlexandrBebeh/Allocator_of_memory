#pragma once
#include <stdint.h>
#define HEADER 8
#define SIZE_OF_MEMORY 1024
#define LEVEL 4

typedef class Allocator {
	char memory[SIZE_OF_MEMORY];

	void new_block(void* addr, size_t size);
	void segmentation(void* addr, size_t size);
	void mem_move_to_right(void* addr1, void* addr2, int dist);
	void mem_move_to_left(void* addr1, void* addr2, int dist);
public:
	Allocator();

	void show_all();
	
	void* mem_alloc(size_t size);
	void* mem_realloc(void* addr, size_t size);
	void mem_free(void* addr);
};
