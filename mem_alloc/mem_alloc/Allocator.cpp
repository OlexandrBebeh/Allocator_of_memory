#include "Allocator.h"
#include <stdio.h>
#include <iostream>

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

        printf("Number of block: %d; Is reserved: %d; Size: %d; Size of prev: %d;\n", i, (bool)*p, *(uint16_t*)((char*)p + 1), *(uint16_t*)((char*)p + 3));

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
   if (size % LEVEL != 0) {
       size = size + (LEVEL - size % LEVEL);
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

            segmentation(p, (size_t)size + HEADER);
            return p + HEADER;
        }
        p = (char*)p + *(uint16_t*)((char*)p + 1);
        if (p + *(uint16_t*)((char*)p + 1) > memory + SIZE_OF_MEMORY) break;
    }

    return nullptr;
}
void Allocator::mem_move_to_right(void* addr1, void* addr2, int dist) {
    char* move_from = (char*)addr1;
    char* move_to = (char*)addr2;
    for (int i = 0; i < dist; i++) {
        *move_to = *move_from;
        move_to--;
        move_from--;
    }
}

void Allocator::mem_move_to_left(void* addr1, void* addr2, int dist) {
    char* move_from = (char*)addr1;
    char* move_to = (char*)addr2;
    for (int i = 0; i < dist; i++) {
        *move_to = *move_from;
        move_to++;
        move_from++;
    }
}

void* Allocator::mem_realloc(void* addr, size_t size)
{

    if (addr == nullptr) {
        return mem_alloc(size);
    }
    char *p = (char*)addr - HEADER;

    if (size % LEVEL != 0) {
        size = size + (LEVEL - size % LEVEL);
    }

    if (size + HEADER < *(uint16_t*)(p + 1)) {
        segmentation(((char*)p), (size_t)size + HEADER);
        return addr;
    }

    if (size + HEADER == *(uint16_t*)(p + 1)) {
        return addr;
    }

    if (*(bool*)(p + *(uint16_t*)(p + 1)) == true && *(bool*)(p - *(uint16_t*)(p + 3)) == true)
        return mem_alloc(size);

    if (*(bool*)(p + *(uint16_t*)(p + 1)) == false) {
        if (p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
            if (size + HEADER <= *(uint16_t*)(p + 1) + *(uint16_t*)(p + 1 + *(uint16_t*)(p + 1))) {
                p = p + *(uint16_t*)(p + 1);
                *p = true;

                char* move_from = p - 1;
                int dist = *(uint16_t*)((char*)addr - HEADER + 1) - HEADER;
                uint16_t* p_size = (uint16_t*)((char*)addr - HEADER + 1);
                *p_size = *p_size + *(uint16_t*)((char*)p_size + *p_size);

                 if (p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
                   p = (char*)p_size;
                   p_size = (uint16_t*)((char*)p_size + *p_size + 2);
                   *p_size = *(uint16_t*)(char*)p;
                 }
                 p = (char*)addr - HEADER;
                 segmentation(p, (size_t)size + HEADER);
                 char* move_to = p - 1 + *(uint16_t*)(p + 1);
                 
                 mem_move_to_right(move_from, move_to,dist);
                 return addr;
            }
        }
    }

    if (*(bool*)(p - *(uint16_t*)(p + 3)) == false) {
            if (size + HEADER  <= *(uint16_t*)(p + 1) + *(uint16_t*)(p + 3)) {
                p = p - *(uint16_t*)(p + 3);
                *p = true;
                char* move_from = (char*)addr;
                uint16_t* p_size = (uint16_t*)((char*)p + 1);
                int dist = *(uint16_t*)((char*)addr - HEADER + 1) - HEADER;
                *p_size = *p_size + *(uint16_t*)((char*)addr + 1 - HEADER);
                mem_move_to_left(move_from, (char*)p + HEADER, dist);
                segmentation(p, (size_t)size + HEADER);

                 return p;
            }
    }

    if (p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
        if (size + HEADER <= *(uint16_t*)(p + 1) + *(uint16_t*)(p + 1 + *(uint16_t*)(p + 1)) + *(uint16_t*)(p + 3)) {
            char* move_from = p + *(uint16_t*)(p + 1) - 1;
            char* move_to = p - 1 + * (uint16_t*)(p + 1) + *(uint16_t*)(p + *(uint16_t*)(p + 1));
            int dist = *(uint16_t*)(p + 1) - HEADER;
            p = p - *(uint16_t*)(p + 3);
            mem_free(addr);

            mem_move_to_left(addr, p + HEADER, dist);
            segmentation(p, (size_t)size + HEADER);
            return p;
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
