#pragma once
#include <vector>
#include "MonoBehaviour.h"
#include "Transform.h"
#include "RectTransform.h"
#include "../misc/GUID.h"
#include "../misc/ArrayPtr.h"
class Transform;
class RectTransform;

using namespace std;

class GameObject{
private:
	GameObject(void);
	
	ArrayPtr components;
	//int parent;
	//vector<int> children;
	void dispose(void);
	
public:
	static const int UseNoTransform = 0;
	static const int UseTransform = 1;
	static const int UseRectTransform = 2;
	static const int HierarchyOnly = 3;

	unsigned int guid;

	BaseTransform* _transform;
	char name[64];

	~GameObject();
	static GameObject* instantiate(int mode);

	inline Transform* transform(){
		return (Transform*)_transform;
	}
	inline RectTransform* rectTransform(){
		return (RectTransform*)_transform;
	}
	void addComponent(MonoBehaviour* mono);
	void setName(const char *_name);

	void awake(void);
	void start(void);
	void update(void);
	void fixedUpdate(void);
	void onDestroy(void);
};