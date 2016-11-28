#pragma once
//#include <vector>
#include "../misc/ArrayPtr.h"
class GameObject;
//using namespace std;
class BaseTransform{
protected:
	GameObject* gameObject;

	BaseTransform* parent;
	ArrayPtr children;
public:
	void c(void);
	void d(void);
	void setGameObject(GameObject* _go);
	void setParent(BaseTransform* _transform);
	void addChild(BaseTransform* _transform);
	//void addChild(BaseTransform* _transform);
	GameObject* getGameObject(void);
	inline BaseTransform* getParent(){
		return parent;
	}
};
