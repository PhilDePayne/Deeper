#include "SceneGraphNode.h"

void SceneGraphNode::addChild(const nodePtr& sgn)
{

	m_children.emplace_back(sgn);

}

void SceneGraphNode::update(bool dirty) {

        dirty |= m_dirty;

        if (dirty) {

            //globalna zmiana transforma

            m_dirty = false;

        }

        for (const auto& child : m_children) {

            child->update(dirty);

        }

}

void SceneGraphNode::render(bool isRoot = false) {

    if (!isRoot) {

        //funkcja renderera

    }

    for (const auto& child : m_children) {

        child->render();

    }

}

void SceneGraphNode::updateTransform() {

    //lokalna zmiana transforma
    m_dirty = true;

    for (const auto& child : m_children) {

        child->updateTransform();

    }
}
