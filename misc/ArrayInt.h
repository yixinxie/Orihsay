#pragma once
#include <assert.h>
class ArrayInt{
private:
	void resize(void);
	
	int arraySize;
	int* arrayData;
public:
	int length;
	
	ArrayInt(int defaultSize = 4);
	~ArrayInt();
	void clear(void);
	void push(int val); // add at the end of the array.
	void removeAt(int idx); // at index idx
	void remove(int val); // delete the first appearance of the element that equals to 'val'.
	
	int& operator[] (int idx) {
		assert(idx >= 0 && idx < length);
		return arrayData[idx];
	}
};