#pragma once
#include "ArrayInt.h"
ArrayInt::ArrayInt(int defaultSize){
	arraySize = defaultSize;
	arrayData = new int[arraySize];
	length = 0;

}
ArrayInt::~ArrayInt(){
	delete arrayData;
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