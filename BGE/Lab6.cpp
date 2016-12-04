#include "Lab6.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"
#include "Utils.h"


using namespace BGE;

Lab6::Lab6(void)
{
	elapsed = 10000;
	turnRate = glm::half_pi<float>(); // Turn half_pi radians per second
}


Lab6::~Lab6(void)
{
}

bool Lab6::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);	

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	ship1->Attach(make_shared<VectorDrawer>());
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship2->Attach(make_shared<VectorDrawer>());
	ship2->transform->diffuse= glm::vec3(1.0f,0.0f,0.0f);
	ship2->transform->specular = glm::vec3(1.2f, 1.2f, 1.2f);

	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);

	slerping = false;
	t = 0.0f;

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab6::Update(float timeDelta)
{	
	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->Walk(speed * timeDelta);
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->Walk(-speed * timeDelta);
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(timeDelta * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-timeDelta * speed * speed);
	}

	if (keyState[SDL_SCANCODE_O])
	{
		ship2->transform->Fly(timeDelta * speed);
	}

	if (keyState[SDL_SCANCODE_L])
	{
		ship2->transform->Fly(-timeDelta * speed);
	}

	if (keyState[SDL_SCANCODE_SPACE]){

	}

	// Your code goes here...

	if (keyState[SDL_SCANCODE_SPACE]) {

		slerping = true;
	}


	if (slerping) {
		glm::vec3 relPos = glm::normalize(ship2->transform->position - ship1->transform->position);
		glm::vec3 axis = glm::normalize(glm::cross(Transform::basisLook, relPos));
		float angleOfRot = glm::dot(ship1->transform->basisLook, relPos);
		float toRotation = glm::degrees(glm::acos(angleOfRot));
		float timeReq = toRotation / turnRate;

		glm::quat q = glm::angleAxis(toRotation, axis);

		t += timeDelta / timeReq;
		ship1->transform->orientation = glm::mix(ship1->transform->orientation, glm::normalize(q), t);
		cout << t << endl;
		if (t >= 1.0f) {
			slerping = false;
			t = 0;
		}
	}

	Game::Update(timeDelta);

}
