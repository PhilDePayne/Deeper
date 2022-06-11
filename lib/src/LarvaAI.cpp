#include "LarvaAI.h"

void LarvaAI::onCollisionEnter(BoxCollider collided) {

}

void LarvaAI::onTriggerEnter(BoxCollider collided, Tag colliderTag) {

	active = true;

	if (colliderTag == Tag::PICKAXE) {
		if (larvas->size() > 0) {

			larvas->erase(larvas->begin());
			parent->~GameObject();
		}
	}

}

void LarvaAI::update(GLFWwindow* window, float deltaTime) {

	if (active) {
		glm::vec3 currentPos = parent->getComponent<Model>(ComponentType::MODEL)->transform.position;

		glm::vec3 deltaPos = moveTowards(currentPos, lights->at(0), speed * deltaTime);

		parent->getComponent<Model>(ComponentType::MODEL)->transform.position = deltaPos;
		parent->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(deltaPos);
	}

}