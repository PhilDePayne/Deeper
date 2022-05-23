#include "SceneGraphNode.h"
#include "GameObject.h"
#include "CubeMesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <iostream>

SceneGraphNode::~SceneGraphNode() {};

void SceneGraphNode::addGameObject(gameObjectPtr g) {

    gameObject = g;

}

void SceneGraphNode::addChild(const nodePtr& sgn)
{

	children.emplace_back(sgn);

}

void SceneGraphNode::update(gameObjectPtr parent, bool dirty) {

        dirty |= this->dirty;

        if (dirty && (parent != nullptr)) {

            gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix = 
                gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->getCombinedMatrix();

            gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix =
                parent->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix *
                gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix;

            this->dirty = false;

        }

        for (const auto& child : children) {

            child->update(gameObject, dirty);

        }

}

void SceneGraphNode::render(Shader shader, bool isRoot) {

    if (!isRoot) {

        if (gameObject->getComponent<CubeMesh>(ComponentType::CUBEMESH) != nullptr) {
            //TODO: w GameObject
            glm::mat4 model = gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix;
            shader.setMat4("model", model);

            std::vector<float> vertices = gameObject->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVertices();

            glBindVertexArray(gameObject->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVAO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

    }

    for (const auto& child : children) {

        child->render(shader);

    }

}

void SceneGraphNode::updateTransform() {

    if (gameObject->getComponent<Transform>(ComponentType::TRANSFORM) != nullptr) {
        gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix =
            gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->getCombinedMatrix();
    }

    dirty = true;

    for (const auto& child : children) {

        child->updateTransform();

    }
}

gameObjectPtr SceneGraphNode::getGameObject() {

    return gameObject;

}