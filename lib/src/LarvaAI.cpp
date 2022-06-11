#include "LarvaAI.h"

void LarvaAI::onCollisionEnter(BoxCollider collided) {

}

void LarvaAI::onTriggerEnter(BoxCollider collided) {

	active = true;
	printf("%d", larvas->size());
	if(larvas->size() > 0){ 

		parent.~shared_ptr();
		larvas->erase(larvas->begin());

	}
	//printf("%d", parent.use_count());

}

void LarvaAI::update(GLFWwindow* window, float deltaTime) {

	if (active) {
		glm::vec3 currentPos = parent->getComponent<Model>(ComponentType::MODEL)->transform.position;

		glm::vec3 deltaPos = moveTowards(currentPos, lights->at(0), speed * deltaTime);

		parent->getComponent<Model>(ComponentType::MODEL)->transform.position = deltaPos;
		parent->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(deltaPos);
	}

}