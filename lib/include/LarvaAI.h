#pragma once

#include "AI.h"
#include "SphereCollider.h"
#include "typedefs.h"

class Sound;

class LarvaAI : public AI {

public:
	LarvaAI() {};
	LarvaAI(gameObjectPtr parent) : AI(parent) {};
	~LarvaAI() {};

	void onCollisionEnter(BoxCollider collided) override;
	void onTriggerEnter(BoxCollider collided, Tag colliderTag) override;
	void update(GLFWwindow* window, float deltaTime) override;
    void setKillSound(Sound *newKillSound);
    float rotateTowardsTarget(glm::vec3 pos, int dir);

	static void instantiateLarva(std::vector<gameObjectPtr>* larvas, std::vector<glm::vec3>* lightPositions, glm::vec3 pos, int id);

	std::vector<glm::vec3>* lights;
	std::vector<gameObjectPtr>* larvas;
	bool active = 1;

	int id = 0;

private:

	
	float speed = 30.0f;
	Sound *killSound = nullptr;
	int dir = 1;

};