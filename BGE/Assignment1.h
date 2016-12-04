#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include "PhysicsController.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"
#include <ctime>
#include "Gorilla.h"

namespace BGE{
	
	class Assignment1 : 
		public Game{
	private:
		shared_ptr<Gorilla> gorilla;
	public: 
		Assignment1(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();

		float bodySize = 10.0f * 3;
		float headSize = 5.0f * 3;
		float hipSize = headSize - (headSize / 4);
		float armSize = bodySize * 2 - (bodySize / 3);

		int frameCount = 0;
		bool forBack = true;

		glm::vec3 bodyCentre = glm::vec3(0, 20, 0);

		//hingePositions
		btVector3 armHingePos = btVector3(hipSize + 0.5f, armSize / 2 + 0.5f, 0);
		btVector3 bodyHingePos = btVector3(-bodySize - (bodySize / 4) - 0.1f, bodySize / 2 + 0.1f, 0);
		btVector3 armLeftHingePos = btVector3(-(hipSize + 0.5f), armSize / 2 + 0.5f, 0);
		btVector3 bodyLeftHingePos = btVector3(-(-bodySize - (bodySize / 4) - 0.1f), bodySize / 2 + 0.1f, 0);

		//hinge joints
		btHingeConstraint * rightArmHinge;
		btHingeConstraint * rightForeHinge;
		btHingeConstraint * leftArmHinge;
		btHingeConstraint * rightThighHinge;
		btHingeConstraint * leftForeHinge;
		btHingeConstraint * rightCalfHinge;
		btHingeConstraint * leftThighHinge;
		btHingeConstraint * leftCalfHinge;

		//arm rotations
		glm::quat q = glm::angleAxis(-glm::half_pi<float>(), glm::vec3(1, 0, 0));
		glm::quat q1 = glm::angleAxis(glm::half_pi<float>(), glm::vec3(1, 0, 0));

		//arms/legs
			shared_ptr<PhysicsController> armLeft;
			shared_ptr<PhysicsController> armRight;
			shared_ptr<PhysicsController> foreLeft;
			shared_ptr<PhysicsController> foreRight;
			shared_ptr<PhysicsController> calfLeft;
			shared_ptr<PhysicsController> calfRight;
			shared_ptr<PhysicsController> thighRight;
			shared_ptr<PhysicsController> thighLeft;
		
	};
}