#include <iostream>
#include "Allocator.h"

void test()
{
	/*
	uint8_t* p;
	uint8_t *l = new uint8_t (10);
	p = l;
	printf("%d\n", *p);
	p = (uint8_t*)p + 1;
	*p = 8;
	printf("%d\n", (uint8_t)*((uint8_t*)l + 1));*/
	Allocator a;
	double* b;
	b = (double*)a.mem_alloc(sizeof(double));
	*b = 12;
	double* f = (double*)a.mem_alloc(sizeof(double[2]));
	a.mem_free(f);
	b = (double*)a.mem_alloc(sizeof(double));
	a.show_all();
}

int main()
{
	test();
	return;
}
