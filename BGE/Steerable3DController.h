#pragma once
#include "GameComponent.h"
#include "Model.h"

using namespace std;
namespace BGE
{
	class Steerable3DController :
		public GameComponent
	{
	private:
		void Steerable3DController::CalculateInertiaTensor();
		shared_ptr<Model> model;
	public:
		Steerable3DController(shared_ptr<Model> model);
		Steerable3DController(shared_ptr<Model> model, SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, SDL_Keycode rollRight, SDL_Keycode rollLeft, SDL_Keycode rise);
		~Steerable3DController(void);

		SDL_Keycode keyUp = SDL_SCANCODE_UP;
		SDL_Keycode keyLeft = SDL_SCANCODE_LEFT;
		SDL_Keycode keyRight = SDL_SCANCODE_RIGHT;
		SDL_Keycode keyDown = SDL_SCANCODE_DOWN;
		SDL_Keycode keyRollLeft = SDL_SCANCODE_Y;
		SDL_Keycode keyRollRight = SDL_SCANCODE_H;
		SDL_Keycode keyRise = SDL_SCANCODE_SPACE;

		bool Initialise();
		void Update(float timeDelta);
		void Draw();

		float mass;
		glm::vec3 velocity, force, acceleration;
        glm::vec3 angularVelocity, angularAcceleration, torque;
        glm::mat3 inertialTensor;

		void AddForce(glm::vec3);
		void AddTorque(glm::vec3);
		void AddForceAtPoint(glm::vec3, glm::vec3);
	};
}
