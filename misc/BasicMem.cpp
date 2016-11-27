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
}
void* BasicMemory::alloc(int _size){
	unsigned char* ret = heapStart + used;
	used += _size;
	return (void*)ret;
}
void BasicMemory::dealloc(void* ptr){

}