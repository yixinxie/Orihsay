#include "ClassFactory.h"
#include "../components/Panel.h"
#include "../components/Camera.h"
#include "../components/CubeMesh.h"
void registerComponentClasses(void){
	REGISTER_CLASS(Panel);
	REGISTER_CLASS(Camera);
	REGISTER_CLASS(CubeMesh);

}