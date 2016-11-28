#pragma once
#include "ArrayInt.h"
#include "BasicMem.h"
void ArrayInt::c(int defaultSize){
	arraySize = defaultSize;
	arrayData = ori_alloc_array(int, arraySize);
	length = 0;

}
void ArrayInt::d(){
	ori_dealloc(arrayData);
}
void ArrayInt::clear(){
	length = 0;
}
void ArrayInt::push(int val){
	if (length == arraySize){
		resize();
	}
	arrayData[length] = val;
	length++;
}
void ArrayInt::removeAt(int idx){
	assert(idx >= 0 && idx < length);
	length--;
	for (int i = idx; i < length; ++i){
		arrayData[i] = arrayData[i + 1];
	}
}
void ArrayInt::remove(int val){
	for (int i = 0; i < length; ++i){
		if (arrayData[i] == val){
			removeAt(i);
			break;
		}
	}
}
void ArrayInt::resize(){
	int newSize = arraySize * 2;

	int* newArray = new int[newSize];

	for (int i = 0; i < arraySize; ++i){
		newArray[i] = arrayData[i];
	}
	delete arrayData;
	arrayData = newArray;
	arraySize = newSize;
}