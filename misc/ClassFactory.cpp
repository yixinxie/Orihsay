#include "ClassFactory.h"
#include "../components/Camera.h"
#include "../components/CubeMesh.h"
#include "../components/InputManager.h"
#include "../components/LightSource.h"
#include "../components/Panel.h"
#include "../components/RawImage.h"


void registerComponentClasses(void){
	REGISTER_CLASS(Camera);
	REGISTER_CLASS(CubeMesh);
	REGISTER_CLASS(InputManager);
	REGISTER_CLASS(LightSource);
	REGISTER_CLASS(Panel);
	REGISTER_CLASS(RawImage);

}