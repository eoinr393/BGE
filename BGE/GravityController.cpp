#include "GravityController.h"

using namespace BGE;

GravityController::GravityController(){

	gravity = glm::vec3(0, -0.5f, 0);
}

void GravityController::Update(float timeDelta){
	//add force up
	if ((parent->transform->position.y + parent->transform->scale.y <= 0) && parent->transform->velocity.y < 0){
		
		parent->transform->velocity.y = -1 * parent->transform->velocity.y ;
		parent->transform->position.y = 0;
	}
	else{
		parent->transform->position += gravity * timeDelta;
		parent->transform->velocity += gravity * timeDelta;
	}

	parent->transform->position.y += parent->transform->velocity.y;
}