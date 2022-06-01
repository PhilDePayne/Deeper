//
// Created by Wojtek on 01.06.2022.
//

#ifndef DEEPER_ASSIMPGLMHELPERS_H
#define DEEPER_ASSIMPGLMHELPERS_H

#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

class AssimpGLMHelpers
{
public:

    static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

    static inline glm::vec3 GetGLMVec(const aiVector3D& vec);

    static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation);
};

#endif //DEEPER_ASSIMPGLMHELPERS_H
