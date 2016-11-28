#pragma once
#include "ArrayPtr.h"
#include "BasicMem.h"
void ArrayPtr::c(int defaultSize){
	arraySize = defaultSize;
	arrayData = ori_alloc_array(void*, arraySize);
	length = 0;

}
void ArrayPtr::d(){
	ori_dealloc(arrayData);
}
void ArrayPtr::clear(){
	length = 0;
}
void ArrayPtr::push(void* val){
	if (length == arraySize){
		resize();
	}
	arrayData[length] = val;
	length++;
}
void ArrayPtr::removeAt(const int idx){
	assert(idx >= 0 && idx < length);
	length--;
	for (int i = idx; i < length; ++i){
		arrayData[i] = arrayData[i + 1];
	}
}
void ArrayPtr::remove(const void* val){
	for (int i = 0; i < length; ++i){
		if (arrayData[i] == val){
			removeAt(i);
			break;
		}
	}
}
void ArrayPtr::resize(){
	int newSize = arraySize * 2;

	void** newArray = ori_alloc_array(void*, newSize);

	for (int i = 0; i < arraySize; ++i){
		newArray[i] = arrayData[i];
	}
	ori_dealloc(arrayData);
	arrayData = newArray;
	arraySize = newSize;
}