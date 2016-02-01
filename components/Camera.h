#pragma once
#include "../gameplay/MonoBehaviour.h"
class Camera : public MonoBehaviour{
private:
	bool _isMain;
public:
	float fieldOfView;
	float nearPlane;
	float farPlane;
	inline void setMainCamera(bool v){ _isMain = v; };
	inline bool isMain(void){ return _isMain; };
	void awake(void);
	void update(void);
	
};