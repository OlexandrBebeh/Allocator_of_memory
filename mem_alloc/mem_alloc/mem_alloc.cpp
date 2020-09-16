#include <iostream>
#include "Allocator.h"

void test()
{
	Allocator a;
	double* b, *b1, *b2;
	b = (double*)a.mem_alloc(sizeof(double));
	b1 = (double*)a.mem_alloc(sizeof(double));
	b2 = (double*)a.mem_alloc(sizeof(double));
	*b = 12;
	a.show_all();
	double* f = (double*)a.mem_alloc(sizeof(double[12]));
	a.show_all();
	a.mem_free(f);
	a.show_all();
	std::cout << *b;
	a.mem_free(b);
	a.show_all();
	a.mem_free(b1);
	a.show_all();
	a.mem_free(b2);
	a.show_all();
	b = (double*)a.mem_alloc(sizeof(double));
	b1 = (double*)a.mem_alloc(sizeof(double));
	b2 = (double*)a.mem_alloc(sizeof(double[2]));
	a.show_all();
	int *l = (int*)a.mem_realloc(b2, sizeof(int));
	a.show_all();
	double* m = (double*)a.mem_realloc(b2, sizeof(double));
	a.show_all();

}

int main()
{
	test();
	return 0;
}
