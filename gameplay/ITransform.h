#pragma once

class GameObject;
//#include "../math/EasyMath.h"
//#include "glm/gtc/quaternion.hpp"
class ITransform{
public:
	virtual void setGameObject(GameObject* _go) = 0;
	virtual void setParent(int val) = 0;
	virtual void addChild(int val) = 0;
};
