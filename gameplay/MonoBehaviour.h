#pragma once
class GameObject;
class MonoBehaviour{
private:

public:
	GameObject* gameObject;
	MonoBehaviour();
	virtual void awake(void);
	virtual void start(void);
	virtual void update(void);
	virtual void lateUpdate(void);
	virtual void fixedUpdate(void);	
	virtual void onRender(void);
	virtual void onDestroy(void);
};