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

    CalculateBoneTransform(&CurrentAnimation->GetRootNode(), glm::mat4(1.0f), false);
}

void Animator::UpdateAnimation(float dt)
{
    if (playing)
    {
        DeltaTime = dt;
        if (CurrentAnimation)
        {
            CurrentTime += CurrentAnimation->GetTicksPerSecond() * dt;
            if (CurrentTime >= CurrentAnimation->GetDuration())
            {
                CurrentTime = 0.0f;
                playing = false;
                return;
            }
            CurrentTime = fmod(CurrentTime, CurrentAnimation->GetDuration());
            CalculateBoneTransform(&CurrentAnimation->GetRootNode(), glm::mat4(1.0f), false);
        }
    }
}

void Animator::PlayAnimation(Animation *pAnimation)
{
    CurrentAnimation = pAnimation;
    CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform, bool wasFirstBone)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    bool firstBoneFoundNow = false;
    glm::mat4 globalTransformation;

    Bone* Bone = CurrentAnimation->FindBone(nodeName);
    if (Bone)
    {
        Bone->Update(CurrentTime);
        nodeTransform = Bone->GetLocalTransform();
        if (!wasFirstBone)
        {
            nodeTransform = glm::mat4(1.0f);
            wasFirstBone = true;
            firstBoneFoundNow = true;
            globalTransformation = parentTransform * nodeTransform;
        }
    }

    if (!firstBoneFoundNow)
    {
        globalTransformation = parentTransform * nodeTransform;
    }

    auto boneInfoMap = CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].Id;
        glm::mat4 offset = boneInfoMap[nodeName].offsetMatrix;
        FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation, wasFirstBone);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return FinalBoneMatrices;
}

bool Animator::isPlaying()
{
    return playing;
}

void Animator::play()
{
    playing = true;
    CurrentTime = 0.0f;
}
