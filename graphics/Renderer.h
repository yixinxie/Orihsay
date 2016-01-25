#pragma once
#include "../gameplay/Transform.h"
class Renderer{
protected:
	int width;
	int height;
	Transform* mainCameraTransform;
public:
	virtual void init(HWND hwnd, int _width, int _height) = 0;
	virtual void close(void) = 0;

	virtual void initInstancing(void) = 0;
	virtual void disposeInstancing(void) = 0;
	virtual void render(void) = 0;
	inline void setMainCameraTransform(Transform* _trans){ mainCameraTransform = _trans; }
};
class G{
private:
	G(void);
	static G* _instance;
public:
	static G* instance(void);
	Renderer* renderer;
};