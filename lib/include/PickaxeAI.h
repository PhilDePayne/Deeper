#pragma once

#include "AI.h"
#include "Model.h"
#include "typedefs.h"



class PickaxeAI : public AI {

public:
	PickaxeAI() {};
	PickaxeAI(gameObjectPtr parent) : AI(parent) {};
	~PickaxeAI() {};

	void onCollisionEnter(BoxCollider collided) override;
	void onTriggerEnter(BoxCollider collided) override;
	void update(GLFWwindow* window, float deltaTime) override; //TODO: override
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