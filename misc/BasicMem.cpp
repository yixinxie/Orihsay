// basic memory management
#include "BasicMem.h"
//unsigned char* heapStart;
//int used;
//int size;
BasicMemory::BasicMemory(){

}
void BasicMemory::initHeap(int defaultHeapSize){
	size = defaultHeapSize;
	heapStart = new unsigned char[size];
	for (int i = 0; i < size; ++i){
		heapStart[i] = 0;
	}
}
void* BasicMemory::alloc(int _size){
	unsigned char* ret = heapStart;
	if (used == 0){
		int sdf = 0;
	}
	ret += used;
	used += _size;
	return (void*)ret;
}
void BasicMemory::dealloc(void* ptr){

}