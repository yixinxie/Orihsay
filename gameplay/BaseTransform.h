#pragma once
#include <vector>
class GameObject;
using namespace std;
class BaseTransform{
protected:
	GameObject* gameObject;

	GameObject* parent;
	vector<GameObject*> children;
public:
	BaseTransform(void);
	void setGameObject(GameObject* _go);
	void setParent(GameObject* _go);
	void addChild(GameObject* _go);
	GameObject* getGameObject(void);
};
