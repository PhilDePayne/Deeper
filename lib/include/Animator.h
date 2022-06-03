//
// Created by Wojtek on 01.06.2022.
//

#ifndef DEEPER_ANIMATOR_H
#define DEEPER_ANIMATOR_H

#include "glm/glm.hpp"
#include "Animation.h"

class Animator
{
public:
    Animator(Animation* Animation);

    void UpdateAnimation(float dt);
    void PlayAnimation(Animation* pAnimation);
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

    std::vector<glm::mat4> GetFinalBoneMatrices();

private:
    std::vector<glm::mat4> FinalBoneMatrices;
    Animation* CurrentAnimation;
    float CurrentTime;
    float DeltaTime;
};

#endif //DEEPER_ANIMATOR_H
