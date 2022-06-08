#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Model.h"

#include <glm/glm.hpp>

#include <vector>

class BoxCollider;

class SphereCollider : public Component {

public:

	SphereCollider() {};
	SphereCollider(gameObjectPtr parent) : Component(parent) {};
	SphereCollider(glm::vec3 center, float radius);
	~SphereCollider();

	glm::vec3 getCenter();
	float getRadius();

	void setCenter(glm::vec3 c);
	void setRadius(float r);

	glm::vec4 isCollision(BoxCollider* other, bool resolve);

	void separate(std::vector<BoxCollider> colliders);

	bool isType(ComponentType t) override;
	void test() override;

private:

	glm::vec3 center;
	float radius;

};