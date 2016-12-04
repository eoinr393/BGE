#include "Gorilla.h"
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
#include "PhysicsFactory.h"

using namespace BGE;

Gorilla::Gorilla()
{
}

Gorilla::Gorilla(shared_ptr<PhysicsFactory> pFactory){
	physicsFactory = pFactory;
}

bool Gorilla::Initialise()
{
	return true;
}

shared_ptr<PhysicsController> Gorilla::CreateGorilla(glm::vec3 pos){

	//body
	glm::vec3  bodyPos = pos;
	shared_ptr<PhysicsController> bodyFront = physicsFactory->CreateSphere(bodySize, bodyPos, glm::quat(), false, true);

	glm::vec3  bodyBackPos = glm::vec3(0, bodyPos.y, -bodySize * 2 + (bodySize / 4));
	shared_ptr<PhysicsController> bodyBack = physicsFactory->CreateSphere(bodySize - bodySize / 4, bodyBackPos, glm::quat(), false, true);
	//bodyFront->rigidBody->setMassProps(bodySize * bodySize * bodySize, btVector3(0, 0, 0));

	//head
	glm::vec3 headPos = glm::vec3(0, bodyPos.y + bodySize, bodyPos.z);
	shared_ptr<PhysicsController> face = physicsFactory->CreateSphere(headSize, glm::vec3(0, bodyPos.y + bodySize + headSize, bodySize / 2 + headSize), glm::quat(), false, true);
	

	//arms
	glm::vec3  armLeftPos = glm::vec3(headSize * 3, bodyPos.y + headSize / 3, (bodySize / 2) - headSize);
	//shared_ptr<PhysicsController> armLeft = physicsFactory->CreateCylinder(hipSize, armSize, armLeftPos, glm::quat(), false, true);
	armLeft = physicsFactory->CreateCylinder(hipSize, armSize, armLeftPos, glm::quat(), false, true);

	//glm::vec3  armRightPos = glm::vec3(headSize * -3, bodyPos.y + headSize / 3, (bodySize / 2) - headSize);
	glm::vec3  armRightPos = glm::vec3(headSize * -3, bodyPos.y, (bodySize / 2) - headSize);
	armRight = physicsFactory->CreateCylinder(hipSize, armSize, armRightPos, glm::quat(), false, true);

	//foreArms
	//glm::vec3  foreLeftPos = glm::vec3(headSize * 3, armLeftPos.y - armSize - 2.0f, (bodySize / 2) - headSize);
	glm::vec3  foreLeftPos = glm::vec3(headSize * 3, armLeftPos.y - armSize / 2, (bodySize / 2) - headSize);
	foreLeft = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), foreLeftPos, q1, false, true);

	//glm::vec3  foreRightPos = glm::vec3(headSize * -3, armRightPos.y - armSize - 2.0f, (bodySize / 2) - headSize);
	glm::vec3  foreRightPos = glm::vec3(headSize * -3, armRightPos.y - armSize / 2, (bodySize / 2) - headSize);
	foreRight = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), foreRightPos, glm::quat(), false, true);

	//calfs
	glm::vec3  calfLeftPos = glm::vec3(headSize * 3, foreLeftPos.y, bodyBackPos.z);
	calfLeft = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), calfLeftPos, glm::quat(), false, true);

	glm::vec3  calfRightPos = glm::vec3(headSize * -3, foreLeftPos.y, bodyBackPos.z);
	calfRight = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), calfRightPos, glm::quat(), false, true);

	//thighs
	float thighY = bodyBackPos.y - (bodyBackPos.y - (calfLeftPos.y + (armSize - (armSize / 3)) / 2)) / 2 + headSize;
	glm::vec3  thighLeftPos = glm::vec3(headSize * 3, thighY, bodyBackPos.z);
	thighLeft = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), thighLeftPos, glm::quat(), false, true);

	glm::vec3  thighRightPos = glm::vec3(headSize * -3, thighY, bodyBackPos.z);
	thighRight = physicsFactory->CreateCylinder(hipSize - (hipSize / 4), armSize - (armSize / 3), thighRightPos, glm::quat(), false, true);


	//fixed body and head
	btTransform bodyA, bodyB;
	bodyA.setIdentity(); bodyB.setIdentity();
	bodyA.setOrigin(btVector3(0, 0, -bodySize));
	bodyB.setOrigin(btVector3(0, 0, bodySize - bodySize / 4));
	btFixedConstraint * bodyConst = new btFixedConstraint(*bodyFront->rigidBody, *bodyBack->rigidBody, bodyA, bodyB);
	physicsFactory->dynamicsWorld->addConstraint(bodyConst);


	btTransform headA, headB;
	headA.setIdentity(); headB.setIdentity();
	headA.setOrigin(btVector3(0, bodySize, bodySize));
	headB.setOrigin(btVector3(0, -headSize, -headSize));
	btFixedConstraint * headConst = new btFixedConstraint(*bodyFront->rigidBody, *face->rigidBody, headA, headB);
	physicsFactory->dynamicsWorld->addConstraint(headConst);

	rightArmHinge = new btHingeConstraint(*bodyFront->rigidBody, *armRight->rigidBody, bodyHingePos, armHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightArmHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(rightArmHinge);

	rightForeHinge = new btHingeConstraint(*armRight->rigidBody, *foreRight->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightForeHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(rightForeHinge);

	leftArmHinge = new btHingeConstraint(*bodyFront->rigidBody, *armLeft->rigidBody, bodyLeftHingePos, armLeftHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftArmHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(leftArmHinge);

	leftForeHinge = new btHingeConstraint(*armLeft->rigidBody, *foreLeft->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftForeHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(leftForeHinge);

	rightThighHinge = new btHingeConstraint(*bodyBack->rigidBody, *thighRight->rigidBody, bodyHingePos, armHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightThighHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(rightThighHinge);

	rightCalfHinge = new btHingeConstraint(*thighRight->rigidBody, *calfRight->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	rightCalfHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(rightCalfHinge);

	leftThighHinge = new btHingeConstraint(*bodyBack->rigidBody, *thighLeft->rigidBody, bodyLeftHingePos, armLeftHingePos, btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftThighHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(leftThighHinge);

	leftCalfHinge = new btHingeConstraint(*thighLeft->rigidBody, *calfLeft->rigidBody, btVector3(0, -armSize / 2 - hipSize, 0), btVector3(0, hipSize + hipSize / 3, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	leftCalfHinge->setLimit(btScalar(-0.5f), btScalar(0.5f));
	physicsFactory->dynamicsWorld->addConstraint(leftCalfHinge);

	return bodyFront;

}

void Gorilla::Update(float timeDelta)
{

	const Uint8 * keystate = Game::Instance()->GetKeyState();

	if ((int)timeDelta % 60 == 0){
		forBack = !forBack;
	}
	if (forBack){
		rightArmHinge->enableAngularMotor(true, 30.0f, 60.0f);
		//foreRight->transform->orientation = q1;
		foreRight->rigidBody->applyForce(btVector3(0, 0, -30.0f), btVector3(0, 0, 0));

		leftThighHinge->enableAngularMotor(true, 30.0f, 60.0f);
		calfLeft->rigidBody->applyForce(btVector3(0, 0, -30.0f), btVector3(0, 0, 0));

		leftArmHinge->enableAngularMotor(true, -30.0f, -60.0f);
		foreLeft->rigidBody->applyForce(btVector3(0, 0, 15.0f), btVector3(0, 0, 0));

		rightThighHinge->enableAngularMotor(true, -30.0f, -60.0f);
		calfRight->rigidBody->applyForce(btVector3(0, 0, 30.0f), btVector3(0, 0, 0));
		//rightForeHinge->enableAngularMotor(true, -15.0f, -30.0f);
	}
	else{
		rightArmHinge->enableAngularMotor(true, -30.0f, -60.0f);
		foreRight->rigidBody->applyForce(btVector3(0, 0, 30.0f), btVector3(0, 0, 0));

		leftThighHinge->enableAngularMotor(true, -30.0f, -60.0f);
		calfLeft->rigidBody->applyForce(btVector3(0, 0, 30.0f), btVector3(0, 0, 0));

		leftArmHinge->enableAngularMotor(true, 30.0f, 60.0f);
		foreLeft->rigidBody->applyForce(btVector3(0, 0, -30.0f), btVector3(0, 0, 0));

		rightThighHinge->enableAngularMotor(true, 30.0f, 60.0f);
		calfRight->rigidBody->applyForce(btVector3(0, 0, -30.0f), btVector3(0, 0, 0));
		//rightForeHinge->enableAngularMotor(true, -15.0f, -30.0f);
	}

}
