#pragma once
// going to be lazy here and just use the stock d3dx math library.
#include <d3dx11.h>
#include <d3dx10.h>
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
