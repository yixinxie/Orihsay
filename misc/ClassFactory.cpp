#include "ClassFactory.h"
#include "../components/Panel.h"
#include "../components/Camera.h"
void registerComponentClasses(void){
	REGISTER_CLASS(Panel);
	REGISTER_CLASS(Camera);

}