#include "RectTransform.h"
#include "../gameplay/G.h"
#include "glm/gtx/rotate_vector.hpp"
RectTransform::RectTransform(){
	anchorMin = 0;
	anchorMax = 0;
	offsetMin = IntVector2(0, 0);
	offsetMax = IntVector2(0, 0);
	pivot = IntVector2(0, 0);


	/*position = Vector2(0, 0);
	widthHeight = Vector2(0, 0);
	
	rotation = 0;
	scale = Vector2(1, 1);*/
	spriteHandle = G::instance()->renderer->registerSpriteObject();
}
RectTransform::~RectTransform(){
	//BaseTransform::~BaseTransform();
}

void RectTransform::setParent(BaseTransform* _rect){
	BaseTransform::setParent(_rect);
	int parentId = 0;
	if (_rect != nullptr)
		parentId = ((RectTransform*)_rect)->spriteHandle;
	G::instance()->renderer->updateSpriteObjectParent(this->spriteHandle, parentId);
}