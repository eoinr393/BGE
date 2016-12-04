#include "Assignment1.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Capsule.h"
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
#include <ctime>

using namespace BGE;

Assignment1::Assignment1(void)
{
}

bool Assignment1::Initialise()
{
	Attach(physicsFactory->CreateGroundPhysics());
	physicsFactory->CreateCameraPhysics();

	//body
	glm::vec3  bodyPos = glm::vec3(0, 100, 0);
	shared_ptr<PhysicsController> bodyFront = physicsFactory->CreateSphere(bodySize, bodyPos, glm::quat(), true, true);

	glm::vec3  bodyBackPos = glm::vec3(0, bodyPos.y, -bodySize * 2 + (bodySize / 4));
	shared_ptr<PhysicsController> bodyBack = physicsFactory->CreateSphere(bodySize - bodySize / 4, bodyBackPos, glm::quat(), false, true);

	//head
	shared_ptr<GameComponent> head = make_shared<Sphere>(headSize);
	head->Initialise();
	head->transform->position = glm::vec3(0, bodyPos.y + bodySize, bodySize / 2);
	Attach(head);

	//arms
	glm::vec3  armLeftPos = glm::vec3(headSize * 3, bodyPos.y + headSize / 3, (bodySize / 2) - headSize);
	//shared_ptr<PhysicsController> armLeft = physicsFactory->CreateCylinder(hipSize, armSize, armLeftPos, glm::quat(), false, true);
	armLeft = physicsFactory->CreateCylinder(hipSize, armSize, armLeftPos, glm::quat(), false, true);

	//glm::vec3  armRightPos = glm::vec3(headSize * -3, bodyPos.y + headSize / 3, (bodySize / 2) - headSize);
	glm::vec3  armRightPos = glm::vec3(headSize * -3, bodyPos.y, (bodySize / 2) - headSize);
	armRight = physicsFactory->CreateCylinder(hipSize, armSize, armRightPos, glm::quat(), false, true);

	//foreArms
	//glm::vec3  foreLeftPos = glm::vec3(headSize * 3, armLeftPos.y - armSize - 2.0f, (bodySize / 2) - headSize);
	glm::vec3  foreLeftPos = glm::vec3(headSize * 3, armLeftPos.y - armSize/2, (bodySize / 2) - headSize);
	foreLeft = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), foreLeftPos, q1, false, true);
	
	//glm::vec3  foreRightPos = glm::vec3(headSize * -3, armRightPos.y - armSize - 2.0f, (bodySize / 2) - headSize);
	glm::vec3  foreRightPos = glm::vec3(headSize * -3, armRightPos.y - armSize/2, (bodySize / 2) - headSize);
	foreRight = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), foreRightPos, glm::quat(), false, true);

	//calfs
	glm::vec3  calfLeftPos = glm::vec3(headSize * 3, foreLeftPos.y , bodyBackPos.z);
	calfLeft = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), calfLeftPos, glm::quat(), false, true);

	glm::vec3  calfRightPos = glm::vec3(headSize * -3, foreLeftPos.y, bodyBackPos.z);
	calfRight = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), calfRightPos, glm::quat(), false, true);

	//thighs
	float thighY = bodyBackPos.y - (bodyBackPos.y - (calfLeftPos.y + (armSize - (armSize / 3)) / 2))/2 + headSize;
	glm::vec3  thighLeftPos = glm::vec3(headSize * 3, thighY , bodyBackPos.z);
	thighLeft = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), thighLeftPos, glm::quat(), false, true);

	glm::vec3  thighRightPos = glm::vec3(headSize * -3, thighY, bodyBackPos.z);
	thighRight = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), thighRightPos, glm::quat(), false, true);	


	//fixed body
	btTransform bodyA, bodyB;
	bodyA.setIdentity(); bodyB.setIdentity();
	bodyA.setOrigin(btVector3(0, 0, -bodySize));
	bodyB.setOrigin(btVector3(0, 0, bodySize - bodySize / 4));
	btFixedConstraint * bodyConst = new btFixedConstraint(*bodyFront->rigidBody, *bodyBack->rigidBody, bodyA, bodyB);
	dynamicsWorld->addConstraint(bodyConst);

	//btHingeConstraint * rightArmHinge = new btHingeConstraint(*bodyFront->rigidBody, *armRight->rigidBody, bodyHingePos, armHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightArmHinge = new btHingeConstraint(*bodyFront->rigidBody, *armRight->rigidBody, bodyHingePos, armHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightArmHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(rightArmHinge);

	//btHingeConstraint * rightForeHinge = new btHingeConstraint(*armRight->rigidBody, *foreRight->rigidBody, btVector3(0, -armSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightForeHinge = new btHingeConstraint(*armRight->rigidBody, *foreRight->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightForeHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(rightForeHinge);

	leftArmHinge = new btHingeConstraint(*bodyFront->rigidBody, *armLeft->rigidBody, bodyLeftHingePos, armLeftHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftArmHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(leftArmHinge);

	leftForeHinge = new btHingeConstraint(*armLeft->rigidBody, *foreLeft->rigidBody, btVector3(0, -armSize/2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftForeHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(leftForeHinge);

	rightThighHinge = new btHingeConstraint(*bodyBack->rigidBody, *thighRight->rigidBody, bodyHingePos, armHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightThighHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(rightThighHinge);

	rightCalfHinge = new btHingeConstraint(*thighRight->rigidBody, *calfRight->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightCalfHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(rightCalfHinge);

	leftThighHinge = new btHingeConstraint(*bodyBack->rigidBody, *thighLeft->rigidBody, bodyLeftHingePos, armLeftHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftThighHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(leftThighHinge);

	leftCalfHinge = new btHingeConstraint(*thighLeft->rigidBody, *calfLeft->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftCalfHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	dynamicsWorld->addConstraint(leftCalfHinge);

	camera->transform->position = glm::vec3(80, 50, 120);

	start = clock();

	return true;
}

void BGE::Assignment1::Update(float timeDelta)
{	
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;

	if ((int)duration % 60 == 0){
		forBack = !forBack;
	}
	if (forBack){
		rightArmHinge->enableAngularMotor(true, 30.0f, 30.0f);
		//foreRight->transform->orientation = q1;
		foreRight->rigidBody->applyForce(btVector3(0, 0, 15.0f));

		leftThighHinge->enableAngularMotor(true, 30.0f, 30.0f);
		calfLeft->transform->orientation = q1;

		leftArmHinge->enableAngularMotor(true, -30.0f, -30.0f);
		foreLeft->transform->orientation = q;

		rightThighHinge->enableAngularMotor(true, -30.0f, -30.0f);
		calfRight->transform->orientation = q;
		//rightForeHinge->enableAngularMotor(true, -15.0f, -30.0f);
	}
	else{
		rightArmHinge->enableAngularMotor(true, -30.0f, -30.0f);
		foreRight->transform->orientation = q;

		leftThighHinge->enableAngularMotor(true, -30.0f, -30.0f);
		calfLeft->transform->orientation = q;

		leftArmHinge->enableAngularMotor(true, 30.0f, 30.0f);
		foreLeft->transform->orientation = q1;

		rightThighHinge->enableAngularMotor(true, 30.0f, 30.0f);
		calfRight->transform->orientation = q1;
		//rightForeHinge->enableAngularMotor(true, -15.0f, -30.0f);
	}
	//rightForeHinge->enableAngularMotor(true, -30.0f, -30.0f);
	//armLeft->transform->orientation = q;
	//foreLeft->transform->orientation = q1;
	Game::Update(timeDelta);
}

void BGE::Assignment1::Cleanup(){
	Game::Cleanup();
}