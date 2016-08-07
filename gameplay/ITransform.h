#pragma once

class GameObject;
//#include "../math/EasyMath.h"
//#include "glm/gtc/quaternion.hpp"
class ITransform{
public:
	virtual void setGameObject(GameObject* _go) = 0;
};
