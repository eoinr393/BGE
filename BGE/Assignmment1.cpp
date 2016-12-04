#include "Assignment1.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"
#include "Gorilla.h"

using namespace BGE;

Assignment1::Assignment1(void)
{
}

bool Assignment1::Initialise()
{

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	dynamicsWorld->setGravity(btVector3(0, -3, 0));

	gorilla = make_shared<Gorilla>(physicsFactory);
	gorilla->Initialise();


	gorilla->CreateGorilla(glm::vec3(0, 50, 0));

	if (!Game::Initialise()){
		return false;
	}

	camera->transform->position = glm::vec3(80, 50, 120);

	return true;
}

void BGE::Assignment1::Update(float timeDelta)
{	
	gorilla->Update(timeDelta);
	Game::Update(timeDelta);
}

void BGE::Assignment1::Cleanup(){
	Game::Cleanup();
}