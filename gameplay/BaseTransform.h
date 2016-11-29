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
	BaseTransform(void);
	virtual ~BaseTransform(void);
	void setGameObject(GameObject* _go);
	virtual void setParent(BaseTransform* _transform);
	void addChild(BaseTransform* _transform);
	//void addChild(BaseTransform* _transform);
	GameObject* getGameObject(void);
	inline BaseTransform* getParent(){
		return parent;
	}
};
