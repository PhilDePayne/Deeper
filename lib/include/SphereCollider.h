#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class BoxCollider;

class SphereCollider : public Component {

public:

	SphereCollider() {};
	SphereCollider(glm::vec3 center, float radius);
	~SphereCollider();

	glm::vec3 getCenter();
	float getRadius();

	void setCenter(glm::vec3 c);
	void setRadius(float r);

	glm::vec3 isCollision(BoxCollider* other, bool resolve);

	glm::vec3 separate(BoxCollider* other, glm::vec3 tmpCenter, glm::vec3 maxDist);

	bool isType(ComponentType t) override;
	void test() override;

private:

	glm::vec3 center;
	float radius;

};