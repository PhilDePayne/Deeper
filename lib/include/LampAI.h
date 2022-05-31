#pragma once

#include "AI.h"
#include "typedefs.h"

class LampAI : public AI {

public:
	LampAI() {};
	LampAI(gameObjectPtr parent) : AI(parent) {};
	~LampAI() {};

	void onCollisionEnter() override;
	void onTriggerEnter(BoxCollider collided) override;

	std::vector<glm::vec3>* lights;

};