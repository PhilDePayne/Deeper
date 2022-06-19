#include "LarvaAI.h"
#include "Sound.h"

void LarvaAI::onCollisionEnter(BoxCollider collided) {

}

void LarvaAI::onTriggerEnter(BoxCollider collided, Tag colliderTag) {

	active = true;

	if (colliderTag == Tag::PICKAXE) {
		if (larvas->size() > 0) {
            if (killSound != nullptr)
            {
                killSound->play();
            }
			printf("LARVA TRIGGER\n");
			larvas->erase(larvas->begin());
			parent->~GameObject();
		}
	}

}

void LarvaAI::update(GLFWwindow* window, float deltaTime) {

	if (active) {
		glm::vec3 currentPos = parent->getComponent<Transform>(ComponentType::TRANSFORM)->position;

		glm::vec3 deltaPos = moveTowards(currentPos, lights->at(0), speed * deltaTime);

		parent->getComponent<Transform>(ComponentType::TRANSFORM)->position = deltaPos;
		parent->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(parent->getComponent<Transform>(ComponentType::TRANSFORM)->position);
		parent->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(parent->getComponent<Transform>(ComponentType::TRANSFORM)->position);
	}

}

void LarvaAI::instantiateLarva(std::vector<gameObjectPtr>* larvas, std::vector<glm::vec3>* lightPositions, glm::vec3 pos) {

	gameObjectPtr larva(new GameObject());
	larva->tag = Tag::LARVA;
	larvas->push_back(larva);

	//-------- MODEL --------//
	larvas->back()->addComponent<Transform>(larvas->back());
	larvas->back()->getComponent<Transform>(ComponentType::TRANSFORM)->position = pos;
    larvas->back()->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(0.2f);
	//-------- AI --------//
	larvas->back()->addComponent<LarvaAI>(larvas->back());
	larvas->back()->getComponent<LarvaAI>(ComponentType::AI)->lights = lightPositions;
	larvas->back()->getComponent<LarvaAI>(ComponentType::AI)->larvas = larvas;
	//-------- PHYSICS COLLIDER --------//
	larvas->back()->addComponent<SphereCollider>(larvas->back());
	larvas->back()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(10.0f);
	larvas->back()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(larvas->back()->getComponent<Transform>(ComponentType::TRANSFORM)->position);
	//-------- TRIGGER COLLIDER --------//
	larvas->back()->addComponent<BoxCollider>(larvas->back());
	larvas->back()->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(10.0f));
	larvas->back()->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(larvas->back()->getComponent<Transform>(ComponentType::TRANSFORM)->position);

	larva.reset();
}


void LarvaAI::setKillSound(Sound *newKillSound)
{
    killSound = newKillSound;
}

float LarvaAI::rotateTowardsTarget(glm::vec3 pos, int cam_dir) {
    if(cam_dir == 0) {
        if(pos.x > parent->getComponent<Transform>(ComponentType::TRANSFORM)->position.x) {
            return 0.0f;
        }
        else
            return 1.0f;
    }
    else if(cam_dir == 1) {
        if(pos.z < parent->getComponent<Transform>(ComponentType::TRANSFORM)->position.z) {
            return 0.0f;
        }
        else
            return 1.0f;
    }
    else if(cam_dir == 2) {
        if(pos.x < parent->getComponent<Transform>(ComponentType::TRANSFORM)->position.x) {
            return 0.0f;
        }
        else
            return 1.0f;
    }
    else {
        if(pos.z > parent->getComponent<Transform>(ComponentType::TRANSFORM)->position.z) {
            return 0.0f;
        }
        else
            return 1.0f;
    }

    return 0;
}
