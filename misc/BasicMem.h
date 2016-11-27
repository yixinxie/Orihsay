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