#include "GravityController.h"

using namespace BGE;

GravityController::GravityController(){

	gravity = glm::vec3(0, -0.1f, 0);
}

void GravityController::Update(float timeDelta){
	//add force up
	if ((parent->transform->position.y + parent->transform->scale.y <= ground->transform->position.y) && parent->transform->velocity.y < 0){
		parent->transform->velocity.y = 0;
	}
	parent->transform->position += gravity * timeDelta;
	parent->transform->velocity += gravity * timeDelta;
}