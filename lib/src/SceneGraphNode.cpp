#include "SceneGraphNode.h"

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

void SceneGraphNode::render(bool isRoot) {

    if (!isRoot) {

        //funkcja renderera

    }

    for (const auto& child : children) {

        child->render();

    }

}

void SceneGraphNode::updateTransform() {

    gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix =
        gameObject->getComponent<Transform>(ComponentType::TRANSFORM)->getCombinedMatrix();

    dirty = true;

    for (const auto& child : children) {

        child->updateTransform();

    }
}

gameObjectPtr SceneGraphNode::getGameObject() {

    return gameObject;

}