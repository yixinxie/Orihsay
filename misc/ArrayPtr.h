#pragma once
#include <assert.h>
class ArrayPtr{
private:
	void resize(void);
	
	int arraySize;
	void** arrayData;
public:
	int length;
	
	ArrayPtr(int defaultSize = 4);
	~ArrayPtr();
	void clear(void);
	void push(void* val); // add at the end of the array.
	void removeAt(const int idx); // at index idx
	void remove(const void* val); // delete the first appearance of the element that equals to 'val'.
	
	void* operator[] (int idx) {
		assert(idx >= 0 && idx < length);
		return arrayData[idx];
	}
};