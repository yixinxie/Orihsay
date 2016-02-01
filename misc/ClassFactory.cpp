#include "ClassFactory.h"
#include "../components/Camera.h"
#include "../components/CubeMesh.h"
#include "../components/InputManager.h"
#include "../components/Panel.h"


void registerComponentClasses(void){
	REGISTER_CLASS(Camera);
	REGISTER_CLASS(CubeMesh);
	REGISTER_CLASS(InputManager);
	REGISTER_CLASS(Panel);

}