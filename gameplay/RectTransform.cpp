#include "RectTransform.h"
#include "../gameplay/G.h"
#include "glm/gtx/rotate_vector.hpp"
RectTransform::RectTransform(){
	gameObject = nullptr;

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

