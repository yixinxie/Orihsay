// basic memory management
#pragma once

class BasicMemory{
private:
	unsigned char* heapStart;
	int used;
	int size;
public:
	BasicMemory(void);
	void initHeap(int defaultHeapSize);
	void* alloc(int _size);
	void dealloc(void* ptr);
};
extern BasicMemory basicMem;
#define ori_alloc(TYPE) (TYPE*)basicMem.alloc(sizeof(TYPE));
#define ori_alloc_array(TYPE, COUNT) (TYPE*)basicMem.alloc(sizeof(TYPE) * (COUNT));
#define ori_dealloc(PTR) basicMem.dealloc(PTR);
