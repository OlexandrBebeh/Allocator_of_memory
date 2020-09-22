#include <iostream>
#include "Allocator.h"
using namespace std;

void test()
{
	Allocator a;

	a.show_all();

	double* b,*b1,*b2;
	b = (double*)a.mem_alloc(sizeof(double));
	b1 = (double*)a.mem_alloc(sizeof(double[4]));
	b2 = (double*)a.mem_alloc(sizeof(double));
	*b = 12;
	a.show_all();
	int* f = (int*)a.mem_alloc(sizeof(int[12]));
	a.show_all();
	a.mem_free(f);
	a.show_all();
	a.mem_free(b);
	a.show_all();
	a.mem_free(b1);
	a.show_all();
	a.mem_free(b2);
	a.show_all();


	cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";


	b = (double*)a.mem_realloc(nullptr, sizeof(double[4]));
	b1 = (double*)a.mem_realloc(nullptr, sizeof(double[3]));
	b2 = (double*)a.mem_realloc(nullptr, sizeof(double[3]));
	a.show_all();
	a.mem_free(b);
	a.show_all();
	double* m = (double*)a.mem_realloc(b1, sizeof(double[5]));
	a.show_all();
	int *l = (int*)a.mem_realloc(b2, (size_t)954);
	a.show_all();
	int *t = (int*)a.mem_realloc(nullptr, sizeof(int));
	a.show_all();
	int* h = (int*)a.mem_realloc(nullptr, sizeof(int));
	a.show_all();
}

int main()
{
	test();
	return 0;
}
