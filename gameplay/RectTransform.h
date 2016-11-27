#pragma once

class GameObject;
#include "../math/EasyMath.h"
#include "glm/gtc/quaternion.hpp"
#include "BaseTransform.h"
class RectTransform : public BaseTransform{
	
	
public:
	int spriteHandle;
	// Vector2 position; // anchored position
	//Vector2 widthHeight; // sizeDelta
	

	// core values
	unsigned char anchorMin, anchorMax;
	IntVector2 offsetMin, offsetMax;
	IntVector2 pivot; // pivot uv.
	// end of core values.

	/*float rotation; // z rotation around the pivot.
	Vector2 scale;*/

	
	RectTransform(void);
	
	virtual void setParent(BaseTransform* _rect);
};
