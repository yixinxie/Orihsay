#pragma once
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include "CameraParameters.h"
using namespace std;
class Renderer{
protected:
	// screen width and height
	int width;
	int height;

	vector<CameraParameters*> cameras;
public:
	virtual void init(HWND hwnd, int _width, int _height) = 0;
	virtual void close(void) = 0;

	virtual void initInstancing(void) = 0;
	virtual void disposeInstancing(void) = 0;
	virtual void render(void) = 0;
	void setMainCamera(Vector3& pos, Vector3& rot, float fov, float _nearPlane, float _farPlane);
};
class G{
private:
	G(void);
	static G* _instance;
public:
	static G* instance(void);
	Renderer* renderer;
};