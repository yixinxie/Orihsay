// basic memory management
#pragma once
#include <vector>
#include <unordered_map>
class BasicMemory{
private:
	std::unordered_map<int, int> states;
	std::unordered_map<int, int> sizes;
	unsigned char* heapStart;
	int used;
	int size;
public:
	BasicMemory(void);
	void initHeap(int defaultHeapSize);
	void* alloc(int _size);
	void dealloc(void* ptr);
	void report(void);
};
extern BasicMemory basicMem;
#define ori_alloc(TYPE) (TYPE*)basicMem.alloc(sizeof(TYPE));
#define ori_alloc_array(TYPE, COUNT) (TYPE*)basicMem.alloc(sizeof(TYPE) * (COUNT));
#define ori_dealloc(PTR) basicMem.dealloc(PTR);
