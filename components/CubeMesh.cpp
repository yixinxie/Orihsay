#include "CubeMesh.h"
#include "../graphics/Renderer.h"
#include "../gameplay/GameObject.h"
#include "../gameplay/G.h"

void CubeMesh::awake(void){
	// get the graphics API to recognize this particular instance of cube.
	// the graphics API should take care of resizing the instance buffer.
	drawId = G::instance()->renderer->registerInstancedObject();
}
void CubeMesh::update(){
	// send the transform information to the graphics API so it can update its instance buffer.
	G::instance()->renderer->updateInstancedObject(drawId, gameObject->transform->position, gameObject->transform->getEuler(), gameObject->transform->scale);
}