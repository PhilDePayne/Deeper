#pragma once

#include "AI.h"
#include "typedefs.h"

class LampAI : public AI {

public:
	LampAI() {};
	LampAI(gameObjectPtr parent) : AI(parent) {};
	~LampAI() {};

	void onCollisionEnter(BoxCollider collided) override;
	void onTriggerEnter(BoxCollider collided, Tag colliderTag) override;
	void update(GLFWwindow* window, float deltaTime) override;

	std::vector<glm::vec3>* lights;

    std::vector<bool>* lit;

	bool eaten = false;
	std::vector<glm::vec3> eatenLamps;

};