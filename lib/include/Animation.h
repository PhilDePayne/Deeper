//
// Created by Wojtek on 01.06.2022.
//

#ifndef DEEPER_ANIMATION_H
#define DEEPER_ANIMATION_H

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Bone.h"
#include "Model.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:

    Animation() = default;
    Animation(const std::string& animationPath, Model* model);
    ~Animation();

    Bone* FindBone(const std::string& name);

    inline float GetTicksPerSecond() const;
    inline float GetDuration() const;
    inline const AssimpNodeData& GetRootNode();
    inline const std::map<std::string,BoneInfo>& GetBoneIDMap();

private:
    void ReadMissingBones(const aiAnimation* animation, Model& model);
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

    float Duration;
    int TicksPerSecond;
    std::vector<Bone> Bones;
    AssimpNodeData RootNode;
    std::map<std::string, BoneInfo> BoneInfoMap;
};

#endif //DEEPER_ANIMATION_H
