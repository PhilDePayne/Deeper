#include "SpawnerAI.h"

void SpawnerAI::onCollisionEnter(BoxCollider collided) {

}

void SpawnerAI::onTriggerEnter(BoxCollider collided, Tag colliderTag) {

	if (colliderTag == Tag::PLAYER && !used) {

		LarvaAI::instantiateLarva(larvas, lights, larvaModel, collided.getCenter());
		used = true;
		printf("SPAWNED LARVA\n");

	}

}

void SpawnerAI::update(GLFWwindow* window, float deltaTime) {

}