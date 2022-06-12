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

void LarvaAI::instantiateLarva(std::vector<gameObjectPtr>* larvas, std::vector<glm::vec3>* lightPositions, componentPtr model, glm::vec3 pos) {

	gameObjectPtr larva(new GameObject());
	larva->tag = Tag::LARVA;
	larvas->push_back(larva);

	//-------- MODEL --------//
	larvas->back()->addComponent(model, larvas->back());
	larvas->back()->getComponent<Model>(ComponentType::MODEL)->transform.position = pos;
	//-------- AI --------//
	larvas->back()->addComponent<LarvaAI>(larvas->back());
	larvas->back()->getComponent<LarvaAI>(ComponentType::AI)->lights = lightPositions;
	larvas->back()->getComponent<LarvaAI>(ComponentType::AI)->larvas = larvas;
	//-------- PHYSICS COLLIDER --------//
	larvas->back()->addComponent<SphereCollider>(larvas->back());
	larvas->back()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(10.0f);
	larvas->back()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(larvas->back()->getComponent<Model>(ComponentType::MODEL)->transform.position);
	//-------- TRIGGER COLLIDER --------//
	larvas->back()->addComponent<BoxCollider>(larvas->back());
	larvas->back()->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(10.0f));
	larvas->back()->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(larvas->back()->getComponent<Model>(ComponentType::MODEL)->transform.position);

	larva.reset();
}