// basic memory management
#pragma once
#include <vector>
#include <unordered_map>
class BasicMemory{
private:
	std::unordered_map<int, char*> remarks;
	std::unordered_map<int, int> sizes;
	unsigned char* heapStart;
	int used;
	int size;
public:
	BasicMemory(void);
	void initHeap(int defaultHeapSize);
	void* alloc(int _size);
	void* alloc_r(int _size, const char* remarks);
	void dealloc(void* ptr);
	void report(void);
};
extern BasicMemory basicMem;
#define ori_alloc(TYPE) (TYPE*)basicMem.alloc(sizeof(TYPE));
#define ori_alloc_r(TYPE, REMARKS) (TYPE*)basicMem.alloc_r(sizeof(TYPE), (REMARKS));
#define ori_alloc_array(TYPE, COUNT) (TYPE*)basicMem.alloc(sizeof(TYPE) * (COUNT));
#define ori_alloc_array_r(TYPE, COUNT, REMARKS) (TYPE*)basicMem.alloc_r(sizeof(TYPE) * (COUNT), (REMARKS));
#define ori_dealloc(PTR) basicMem.dealloc(PTR);
