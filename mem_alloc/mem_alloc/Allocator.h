#pragma once
#include <stdint.h>
#define HEADER 8
#define SIZE_OF_MEMORY 1024

typedef class Allocator {
	char memory[SIZE_OF_MEMORY];
public:
	Allocator();

	void reset_inf();
	void show_all();
	void new_block(void* addr, size_t size);
	void* mem_alloc(size_t size);
	void* mem_realloc(void* addr, size_t size);
	void mem_free(void* addr);
};
