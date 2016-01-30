#pragma once
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <unordered_map>
#include "CameraParameters.h"
using namespace std;
struct ObjectInstanceTransform{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};
class Renderer{
protected:
	// screen width and height
	int width;
	int height;

	vector<CameraParameters*> cameras;
	int objectIndexIncrementer;
	std::unordered_map<int, ObjectInstanceTransform*> instancedObjects;
public:
	virtual void init(HWND hwnd, int _width, int _height) = 0;
	virtual void dispose(void) = 0;
	virtual void disposeInstancing(void) = 0;
	virtual void render(void) = 0;
	void setMainCamera(Vector3& pos, Vector3& rot, float fov, float _nearPlane, float _farPlane);
	int registerInstancedObject(void);
	void updateInstancedObject(const int id, const Vector3& position, const Vector3& rotation, const Vector3& scale);

};
