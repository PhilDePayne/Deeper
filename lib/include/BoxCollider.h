#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class BoxCollider : public Component {

public:

	BoxCollider() {};
	BoxCollider(glm::vec3 center, glm::vec3 size);
	~BoxCollider();

	glm::vec3 getCenter();
	float getSizeX();
	float getSizeY();
	float getSizeZ();
	float getMinX();
	float getMaxX();
	float getMinY();
	float getMaxY();
	float getMinZ();
	float getMaxZ();

	void setCenter(glm::vec3 c);
	void setSize(glm::vec3 s);

	bool isType(ComponentType t) override;
	void test() override;

private:
	
	glm::vec3 center;
	glm::vec3 size;

};