//
// Created by Wojtek on 01.06.2022.
//

#include "Animator.h"
#include "Animation.h"

Animator::Animator(Animation *Animation)
{
    CurrentTime = 0.0;
    CurrentAnimation = Animation;

    FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
    DeltaTime = dt;
    if (CurrentAnimation)
    {
        CurrentTime += CurrentAnimation->GetTicksPerSecond() * dt;
        CurrentTime = fmod(CurrentTime, CurrentAnimation->GetDuration());
        CalculateBoneTransform(&CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(Animation *pAnimation)
{
    CurrentAnimation = pAnimation;
    CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* Bone = CurrentAnimation->FindBone(nodeName);
    if (Bone)
    {
        Bone->Update(CurrentTime);
        nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].Id;
        glm::mat4 offset = boneInfoMap[nodeName].offsetMatrix;
        FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return FinalBoneMatrices;
}
