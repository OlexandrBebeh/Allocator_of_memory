#include "Allocator.h"
#include <stdio.h>

Allocator::Allocator() {
    char* p = memory;
    *p = false;
    p = (char*)p + 1;
    uint16_t* pp = (uint16_t*)p;
    *pp = SIZE_OF_MEMORY;
    pp = (uint16_t*)pp + 1;
    *pp = 0;
}

void Allocator::show_all() {
    char* p = memory;
    int i = 0;
    printf("====================================================================\n");
    while (true)
    {

        i++;

        printf("Number of block: %d; Is reserved: %d; Size: %d; Size of prev: %d\n", i, (bool)*p, *(uint16_t*)((char*)p + 1), *(uint16_t*)((char*)p + 3));

        if (*(uint16_t*)((char*)p + 1) + (char*)p >= memory + SIZE_OF_MEMORY) {

            break;
        }

        p = (char*)p + *(uint16_t*)((char*)p + 1);
    }

}

void Allocator::new_block(void* addr, size_t size) {

    char* p = (char*)((char*)addr + size);
    *p = false;
    p = (char*)p + 1;
    uint16_t* pp = (uint16_t*)p;
    *pp = *(uint16_t*)((char*)addr + 1) - size;
    pp = (uint16_t*)pp + 1;
    *pp = size;
    if (p + *(uint16_t*)(char*)p > memory + SIZE_OF_MEMORY) {
        return;
    }
    else {
        pp = (uint16_t*)(p + *(uint16_t*)(char*)p + 2);
        *pp = *(uint16_t*)(char*)p;
    }
}

void Allocator::segmentation(void* addr, size_t size) {
    char* p = (char*)addr;
    *p = true;
    p = (char*)p + 1;
    uint16_t* p_size;
    p_size = (uint16_t*)p;
    if(*(uint16_t*)(char*)p - size >= HEADER){
    new_block(((char*)p - 1), (size_t)size);
    *p_size = size;
    }
}

void* Allocator::mem_alloc(size_t size)
{
   if (size % 4 != 0) {
       size = size + (4 - size % 4);
   }

    char* p = memory;
    uint16_t* p_size;

    while (true) {
        if (p > memory + 1024) break;
        if (*p == false && *(uint16_t*)((char*)p + 1) == size + HEADER) {

            *p = true;
            return p + HEADER;
        }
        if (*p == false && *(uint16_t*)((char*)p + 1) > size + HEADER) {

            segmentation(p, (size_t)size+ HEADER);
            return p + HEADER;
        }
        p = (char*)p + *(uint16_t*)((char*)p + 1);
        if (p + *(uint16_t*)((char*)p + 1) > memory + SIZE_OF_MEMORY) break;
    }

    return nullptr;
}
void Allocator::mem_move(void* addr1, void* addr2) {
    char* p = (char*)addr1 + HEADER;
    char* move_to = (char*)addr2;
    for (int i = 0; i < *(uint16_t*)((char*)addr1 + 1) - HEADER; i++) {
        *move_to = *p;
        move_to++;
        p++;
    }
}

void* Allocator::mem_realloc(void* addr, size_t size)
{

    if (addr == nullptr) {
        return mem_alloc(size);
    }
    char *p = (char*)addr - HEADER;

    if (size % 4 != 0) {
        size = size + (4 - size % 4);
    }

    if (size + HEADER < *(uint16_t*)(p + 1)) {
        void* block = mem_alloc(*(uint16_t*)(p + 1) - HEADER);
        if (block == nullptr) return nullptr;
        mem_move(p, block );
        *p = true;
        segmentation(((char*)p), (size_t)size + HEADER);
        return block;
    }

    if (size + HEADER == *(uint16_t*)(p + 1)) {
        void* block = mem_alloc(*(uint16_t*)(p + 1) - HEADER);
        if (block == nullptr) return nullptr;
        mem_move(p, block);

        return block;
    }

    if (size + HEADER <= *(uint16_t*)(p + 1) + *(uint16_t*)(p + 3)) {
        p = p + *(uint16_t*)(p + 1);
        if (*p == false) {
            *p = true;
            void* block = mem_alloc(*(uint16_t*)((char*)addr - HEADER + 1) - HEADER);
            if (block == nullptr) return nullptr;
            mem_move(p, block);

            uint16_t* p_size = (uint16_t*)((char*)addr - HEADER + 1);

            *p_size = *p_size + *(uint16_t*)((char*)p_size + *p_size);

            if (p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
                p = (char*)p_size;
                p_size = (uint16_t*)((char*)p_size + *p_size + 2);
                *p_size = *(uint16_t*)(char*)p;
            }
            return block;

        }
        
    }
    return nullptr;
}

void Allocator::mem_free(void* addr)
{
    char* p = (char*)addr - HEADER;

    *p = false;

    if (*(bool*)((char*)p + *(uint16_t*)((char*)p + 1)) == false && p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
        uint16_t* p_size = (uint16_t*)((char*)p + 1);

        *p_size = *p_size + *(uint16_t*)((char*)p_size + *p_size);

        if(p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY){
            p_size = (uint16_t*)((char*)p_size + *p_size + 2);
            *p_size = *(uint16_t*)((char*)p + 1);
        }
    }

    if (*(bool*)((char*)p - *(uint16_t*)((char*)p + 3)) == false && *(uint16_t*)((char*)p + 3) != 0) {
        uint16_t* p_size = (uint16_t*)(p - *(uint16_t*)((char*)p + 3) + 1);

        *p_size = *p_size + *(uint16_t*)((char*)p + 1);
        if (p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
            p = (char*)p_size;
            p_size = (uint16_t*)((char*)p_size + *p_size + 2);
            *p_size = *(uint16_t*)((char*)p);
        }
    }
}
