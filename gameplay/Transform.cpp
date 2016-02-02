#include "Transform.h"
#include "../glm/gtx/rotate_vector.hpp"
Transform::Transform(){
	
	position = Vector3(0, 0, 0);
	//rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
}
void Transform::fromEuler(const Vector3 euler){
	quaternion = glm::quat(glm::vec3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z)));
}
void Transform::setGameObject(GameObject* _gameObject){
	gameObject = _gameObject;
}
Vector3 Transform::getEuler(){
	glm::vec3 euler = glm::eulerAngles(quaternion);
	
	return Vector3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
}
void Transform::rotate(const Vector3 axis, const float degree){

	glm::quat q = glm::angleAxis(glm::radians(degree), glm::vec3(axis.x, axis.y, axis.z));
	quaternion = q * quaternion;
}

Vector3 Transform::getLookat(){
	Vector3 euler = getEuler();
	glm::vec3 res = glm::rotateX(glm::vec3(0, 0, 1), glm::radians(euler.x));
	res = glm::rotateY(res, glm::radians(euler.y));
	res = glm::rotateZ(res, glm::radians(euler.z));
	return Vector3(res.x, res.y, res.z);
}
Vector3 Transform::getRight(){
	Vector3 euler = getEuler();
	glm::vec3 res = glm::rotateX(glm::vec3(1, 0, 0), glm::radians(euler.x));
	res = glm::rotateY(res, glm::radians(euler.y));
	res = glm::rotateZ(res, glm::radians(euler.z));
	return Vector3(res.x, res.y, res.z);
}
//
//Vector3 Transform::getUp(){
//	Vector3 euler = getEuler();
//	glm::vec3 res = glm::rotateX(glm::vec3(0, 1, 0), euler.x);
//	res = glm::rotateY(res, euler.y);
//	res = glm::rotateZ(res, euler.z);
//	return Vector3(res.x, res.y, res.z);
//}

// get the lookat and up direction from euler angles(in degrees)
void Transform::getLookatAndUp(const Vector3& euler, Vector3* lookat, Vector3* up){

	glm::vec3 res = glm::rotateX(glm::vec3(0, 0, 1), glm::radians(euler.x));
	res = glm::rotateY(res, glm::radians(euler.y));
	res = glm::rotateZ(res, glm::radians(euler.z));
	lookat->x = res.x;
	lookat->y = res.y;
	lookat->z = res.z;

	res = glm::rotateX(glm::vec3(0, 1, 0), glm::radians(euler.x));
	res = glm::rotateY(res, glm::radians(euler.y));
	res = glm::rotateZ(res, glm::radians(euler.z));
	up->x = res.x;
	up->y = res.y;
	up->z = res.z;
}