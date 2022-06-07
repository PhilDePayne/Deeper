#pragma once

#include "AI.h"
#include "typedefs.h"

#include <GLFW/glfw3.h>

class PickaxeAI : public AI {

public:
	PickaxeAI() {};
	PickaxeAI(gameObjectPtr parent) : AI(parent) {};
	~PickaxeAI() {};

	void onCollisionEnter() override;
	void onTriggerEnter(BoxCollider collided) override;
	void update(GLFWwindow* window); //TODO: override
	void onKeyPressed(int key, int action); //TODO: override
	
	glm::vec3 playerPos;
	int playerDir;
	int throwDir;
	int playerFacingDir;
	int throwFacingDir;

private:
	
	double throwTime;
	bool isThrown = false;

	void pickaxeThrow(int dir, int orientation);
	
};