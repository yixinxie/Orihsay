#include "RectTransform.h"
#include "../gameplay/G.h"
#include "glm/gtx/rotate_vector.hpp"
RectTransform::RectTransform(){
	gameObject = nullptr;

	anchorMin = 0;
	anchorMax = 0;
	offsetMin = IntVector2(0, 0);
	offsetMax = IntVector2(0, 0);
	pivot = Vector2(0.5f, 0.5f);


	/*position = Vector2(0, 0);
	widthHeight = Vector2(0, 0);
	
	rotation = 0;
	scale = Vector2(1, 1);*/
	G::instance()->renderer->registerSpriteObject();
}

