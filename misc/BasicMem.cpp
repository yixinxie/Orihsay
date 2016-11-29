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
	sizes.insert({ (int)ret, _size });
	return (void*)ret;
}
void* BasicMemory::alloc_r(int _size, const char* _remarks){
	unsigned char* ret = heapStart;
	if (used == 0){
		int sdf = 0;
	}
	ret += used;
	used += _size;
	sizes.insert({ (int)ret, _size });
	char *dbg = new char[64];
	strcpy_s(dbg, 64, _remarks);
	remarks.insert({ (int)ret, dbg });
	return (void*)ret;
}
void BasicMemory::dealloc(void* ptr){
	int sdf = 0;
	auto res = sizes.find((int)ptr);
	if (res == sizes.end()){
		// key not found!
		printf("allocation not found %d.\n", (int)ptr);
	}else if (sizes[(int)ptr] == 0){
		printf("repeated deallocation at %d.\n", (int)ptr);
	}
	else{
		sizes[(int)ptr] = 0;
	}
}
void BasicMemory::report(void){
	int leakedCount = 0;
	for (auto it = sizes.begin(); it != sizes.end(); ++it){
		printf("%d size %d", it->first, it->second);
		if (it->second != 0){
			leakedCount++;
		}
		auto found = remarks.find(it->first);
		if (found != remarks.end()){
			printf(" remark: %s\n",  remarks[it->first]);
		}
		else{
			printf("\n");

		}
		
	}
	printf("leak count: %d\n", leakedCount);
	
}