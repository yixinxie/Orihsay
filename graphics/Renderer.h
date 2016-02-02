#pragma once
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <unordered_map>
#include "CameraDesc.h"
#include "LightSourceDesc.h"
#include "ObjectTransformDesc.h"
using namespace std;
namespace OriGraphics{

	/* 
	the Renderer class serves as an intermediary class between the graphics APIs(directx,
	opengl etc) that manages the states of all rendering-related objects.

	*/
	class Renderer{
	protected:
		// screen width and height
		int width;
		int height;

		// while in window mode, the renderable area is slightly smaller than the area the window occupies.
		int windowWidth;
		int windowHeight;

		vector<CameraDesc*> cameras;
		int objectIndexIncrementer;
		std::unordered_map<int, ObjectTransformDesc*> instancedObjects;
	public:
		virtual void init(HWND hwnd, int _width, int _height) = 0;
		virtual void dispose(void) = 0;
		virtual void render(void) = 0;
		void setMainCamera(const Vector3& pos, const Vector3& rot, const float fov, const float _nearPlane, const float _farPlane);
		void updateMainCamera(const Vector3& pos, const Vector3& rot);
		int registerInstancedObject(void);
		void updateInstancedObject(const int id, const Vector3& position, const Vector3& rotation, const Vector3& scale);

	};
}