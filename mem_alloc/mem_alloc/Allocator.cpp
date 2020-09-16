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

void* Allocator::mem_alloc(size_t size)
{
    if (size % 4 == 0) {
        size = size + HEADER;
    }
    else size = size + (4 - size % 4) + HEADER;

    char* p = memory;
    uint16_t* p_size;

    while (true) {
        if (p > memory + 1024) break;
        if (*p == false && *(uint16_t*)((char*)p + 1) == size) {

            *p = true;
            p = (char*)p + 1;
            p_size = (uint16_t*)p;
            *p_size = size;
            return p + HEADER - 1;
        }
        if (*p == false && *(uint16_t*)((char*)p + 1) > size) {

            *p = true;
            p = (char*)p + 1;
            p_size = (uint16_t*)p;
            new_block(((char*)p - 1), (size_t)size);
            *p_size = size;
            return p + HEADER - 1;
        }
        p = (char*)p + *(uint16_t*)((char*)p + 1);
        if (p + *(uint16_t*)((char*)p + 1) > memory + SIZE_OF_MEMORY) break;
    }

    return nullptr;
}

void* Allocator::mem_realloc(void* addr, size_t size)
{
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
        printf("%d", *p_size);
        if (p + *(uint16_t*)((char*)p + 1) < memory + SIZE_OF_MEMORY) {
            p = (char*)p_size;
            printf("%d", (uint16_t*)*p);
            p_size = (uint16_t*)((char*)p_size + *p_size + 2);
            *p_size = *(uint16_t*)((char*)p);
        }
    }
}
