#pragma once

class GameObject;
#include "../math/EasyMath.h"

class Transform{
private:
	
	GameObject* gameObject;

public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	
	Transform(void);
	void setGameObject(GameObject* _gameObject);
	//inline Vector3 getPosition(){ return Vector3(position);}

};
