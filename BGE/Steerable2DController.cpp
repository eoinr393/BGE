#include "Steerable2DController.h"
#include "Content.h"
#include "Model.h"
#include "Utils.h"
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <sstream>
#include <string>

using namespace BGE;
using namespace std;

Steerable2DController::Steerable2DController(){

}

Steerable2DController::Steerable2DController(shared_ptr<Model> model) :GameComponent()
{
	mass = 10.0f;
	velocity = glm::vec3(0);
	force = glm::vec3(0);
	acceleration = glm::vec3(0);
	this->model = model;
}

Steerable2DController::~Steerable2DController(void)
{
}

bool Steerable2DController::Initialise()
{
	if (!model->initialised)
	{
		model->Initialise();
	}

	GameComponent::Initialise();
	return true;
}

void Steerable2DController::AddForce(glm::vec3 force)
{
	this->force += force;
}

void Steerable2DController::Draw()
{
	GameComponent::Draw();
}

void Steerable2DController::Update(float timeDelta)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();

	float scale = 10000.0f;
	if (keyState[keyUp])
	{
		AddForce(glm::vec3(5,0,0));
	}
	// Yaw
	if (keyState[keyRight])
	{
		//AddTorque(transform->up * scale * timeDelta);
		AddForce(glm::vec3(0,0,5));
	}
	if (keyState[keyLeft])
	{
		//AddTorque(-transform->up * scale * timeDelta);
		AddForce(glm::vec3(0, 0, -5));
	}
	if (keyState[keyDown])
	{
		//AddTorque(-transform->up * scale * timeDelta);
		AddForce(glm::vec3(-5, 0, 0));
	}
	// End of Yaw

	// Do the Newtonian integration
	acceleration = force / mass;
	transform->velocity += acceleration * timeDelta;
	transform->position += velocity * timeDelta;

	// Normalise the velocity into the look
	// Probably not necessary as we recalculate these anyway later
	if (glm::length(velocity) > 0.0001f)
	{
		//transform->look = glm::normalize(velocity);
		transform->right = glm::cross(transform->look, transform->up);
		velocity *= 0.99f;
	}

	glm::quat w = glm::quat(0, angularVelocity.x, angularVelocity.y, 0);
	// Reset the accumulators
	force = glm::vec3(0);

	GameComponent::Update(timeDelta);
}