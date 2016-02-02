#pragma once

class GameObject;
#include "../math/EasyMath.h"
#include "../glm/gtc/quaternion.hpp"

class Transform{
private:
	
	GameObject* gameObject;
	glm::quat quaternion;
public:
	Vector3 position;
	//
	Vector3 scale;
	
	Transform(void);
	void setGameObject(GameObject* _gameObject);

	void rotate(const Vector3 axis, const float degree);
	void fromEuler(const Vector3 euler);
	Vector3 getEuler(void);
	Vector3 getLookat(void);
	Vector3 getUp(void);
	Vector3 getRight(void);
	static void getLookatAndUp(const Vector3& euler, Vector3* lookat, Vector3* up);
};
