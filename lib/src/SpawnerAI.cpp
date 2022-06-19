#include "SpawnerAI.h"

void SpawnerAI::onCollisionEnter(BoxCollider collided) {

}

void SpawnerAI::onTriggerEnter(BoxCollider collided, Tag colliderTag) {

	used = false;
	for (auto collider : usedSpawners) {
		if (collided.getCenter() == collider) used = true;
	}

	if (colliderTag == Tag::PLAYER && !used) {

		printf("INSTANTIATE LARVA\n");
		LarvaAI::instantiateLarva(larvas, lights, collided.getCenter());
		usedSpawners.push_back(collided.getCenter());

	}

}

void SpawnerAI::update(GLFWwindow* window, float deltaTime) {

}