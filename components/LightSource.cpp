#include "LightSource.h"
#include "../gameplay/G.h"
#include "../gameplay/GameObject.h"
void LightSource::awake(){
	drawId = G::instance()->renderer->registerLightSource();
}
void LightSource::update(){
	G::instance()->renderer->updateLightSource(drawId, gameObject->transform->position,  gameObject->transform->getEuler());
}