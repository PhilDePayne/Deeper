#include "LampAI.h"

void LampAI::onCollisionEnter() {

	printf("ENTERED COLLISION\n");

};

void LampAI::onTriggerEnter(BoxCollider collided) {

	//printf("TRIGGERED COLLISION\n");
	lights->erase(lights->begin());
	glm::vec3 newLightPos = collided.getCenter();
	lights->push_back(newLightPos);

}
