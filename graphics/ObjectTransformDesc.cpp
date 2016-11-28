#pragma once
#include "ObjectTransformDesc.h"
void ObjectRectTransformDesc::c(){
	dirty = 1;
	parent = -1;
	children.c();
}
void ObjectRectTransformDesc::d(){
	children.d();
}
