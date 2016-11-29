#pragma once
#include "ObjectTransformDesc.h"
ObjectRectTransformDesc::ObjectRectTransformDesc(){
	dirty = 1;
	parent = -1;
}
ObjectRectTransformDesc::~ObjectRectTransformDesc(){
	children.~ArrayInt();
}
