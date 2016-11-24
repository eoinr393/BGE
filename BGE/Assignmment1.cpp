#include "Assignment1.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"

using namespace BGE;

Assignment1::Assignment1(void)
{
}

bool Assignment1::Initialise()
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	shared_ptr<PhysicsController> box1 = physicsFactory->CreateBox(1, 1, 4, glm::vec3(5, 5, 0), glm::quat());
	shared_ptr<PhysicsController> box2 = physicsFactory->CreateBox(1, 1, 4, glm::vec3(5, 5, 5), glm::quat());

	physicsFactory->CreateWall(glm::vec3(0, 0, 0), 10, 5);

	return true;
}

void BGE::Assignment1::Update( float timeDelta)
{
	Game::Update(timeDelta);
}

void BGE::Assignment1::Cleanup(){
	Game::Cleanup();
}