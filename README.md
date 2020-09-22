# Allocator_of_memory

##Allocator of memory on C++ <h2> tag

The class allocator which can select data in heap, with size that you say. I try to use minimum of variables, so you can see there many operations with pointers and access to them.
Memory is divided on **blocks**, every block contains **HEADER** and **memory**. **HEADER** has fixed size 8 bytes. That mean if need to allocate *8* bytes of memory actually needs *16* bytes free memory in one block.
HEADER consist of: 
- 1 byte - is bool, and show is this block free.
- 2 bytes - is size of whole block.
- 2 bytes - is size of previous block.
- 3 bytes - empty, they actually for leveling,you can use them on your own.


*SIZE_OF_MEMORY* - it is how many bytes reserved for your needs. Default 1024 bytes or 1 KB.
Size of **HEADER** you can change for your needs.
All blocks are leveling by 4. That mean if we need *5* bytes in will up to *8*.

Functions:

`void* mem_alloc(size_t size)` – search a free block with enought memory(don't forget that size we need is **HEADER** + **memory** that you give). When finds it a new create block of given size and return pointer to the memory you can use. 
If in block more memory that we need devide it on 2 blocks. 
If there is no block with enough memory return nullptr.  
 
`void mem_free(void* addr)` – mark this block as free. If left or right blocks free so, then unites them.
	
`void* mem_realloc(void* addr, size_t size)` – change size of chosen block, can make it smaller or bigger(if neighboring blocks access to do it). Old memory moves to new block. 
If not enough memory in this block with free adjacent blocks then work as `mem_alloc`.
If there not enough memory for moving of old block returns nullptr. 
If change of size successful returns pointer to the new block with old data. 
If `addr = nullptr` work as `mem_alloc`.

##Usege <h3> tag

To use the algorithm, open the file in any IDE with C++ support.
Describe your use cases in main() function or implement it in separate function, then calling it in main() function.

##Examples <h3> tag

> I'm use function `show_all()` to demonstrate you state of blocks.

Let's start with simple example, like creation of allocator.

Code

```
	Allocator a;

	a.show_all();
```

Output

![example 1](/images/1.png)

Next lets try to alloc some momery for our needs.

Code

```
	double* b,*b1,*b2;
	b = (double*)a.mem_alloc(sizeof(double));
	b1 = (double*)a.mem_alloc(sizeof(double[4]));
	b2 = (double*)a.mem_alloc(sizeof(double));
```

Output

![example 2](/images/2.png)

If we don't need this memory lets make it free.

Code

```
	a.mem_free(b);
	a.show_all();
	a.mem_free(b1);
	a.show_all();
	a.mem_free(b2);
	a.show_all();
```
Output

![example 3](/images/3.png)

As you can see all blocks merge into 1 block after free of all blocks.

As I say `mem_realloc` we can use like `mem_alloc`. 

Code

```
	b = (double*)a.mem_realloc(nullptr, sizeof(double[4]));
	b1 = (double*)a.mem_realloc(nullptr, sizeof(double[3]));
	b2 = (double*)a.mem_realloc(nullptr, sizeof(double[3]));
	a.show_all();
```
Output

![example 4](/images/4.png)

Now I'm gonna free one block and `mem_realloc` another block, which adjacent of free block.

Code

```
	a.mem_free(b);
	a.show_all();
	double* m = (double*)a.mem_realloc(b1, sizeof(double[5]));
	a.show_all();
```
Output

![example 5](/images/5.png)

We can see that `mem_realloc` use adjacent block to expand block that we need and devide it into 2 - one our and another free.

Another example, when we have 2 free adjacent blocks and `mem_realloc` use bouth of them.

Code

```
	a.show_all();
	int *l = (int*)a.mem_realloc(b2, (size_t)954);
	a.show_all();
```
Output

![example 6](/images/6.png)

Last example when we don't have memory and nothing allocate.

Code

```
	int *t = (int*)a.mem_realloc(nullptr, sizeof(int));
	a.show_all();
	int* h = (int*)a.mem_realloc(nullptr, sizeof(int));
	a.show_all();
```
Output

![example 7](/images/7.png)

