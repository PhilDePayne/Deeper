#pragma once

#include "AI.h"
#include "LarvaAI.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "typedefs.h"

class SpawnerAI : public AI {

public:

	SpawnerAI() {};
	SpawnerAI(gameObjectPtr parent) : AI(parent) {};
	~SpawnerAI() {};

	void onCollisionEnter(BoxCollider collided) override;
	void onTriggerEnter(BoxCollider collided, Tag colliderTag) override;
	void update(GLFWwindow* window, float deltaTime) override;

	std::vector<glm::vec3>* lights;
	std::vector<gameObjectPtr>* larvas;

private:

	bool used = false;
	std::vector<glm::vec3> usedSpawners;
	int spawnedLarvas = 0;
};