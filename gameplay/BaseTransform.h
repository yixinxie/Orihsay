#pragma once
#include <vector>
class GameObject;
using namespace std;
class BaseTransform{
protected:
	GameObject* gameObject;

	BaseTransform* parent;
	vector<BaseTransform*> children;
public:
	BaseTransform(void);
	void setGameObject(GameObject* _go);
	virtual void setParent(BaseTransform* _transform);
	void addChild(BaseTransform* _transform);
	//void addChild(BaseTransform* _transform);
	GameObject* getGameObject(void);
	inline BaseTransform* getParent(){
		return parent;
	}
};
