#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float scale;

const int MAX_BONES = 100;

uniform mat4 bones[MAX_BONES];
//uniform mat4 bonesOffsetMats[MAX_BONES];

void main()
{
    mat4 bone_transformation = bones[aBoneIDs[0]] * aWeights[0];
    bone_transformation += bones[aBoneIDs[1]] * aWeights[1];
    bone_transformation += bones[aBoneIDs[2]] * aWeights[2];
    bone_transformation += bones[aBoneIDs[3]] * aWeights[3];

//    mat4 offsetTransformation = bonesOffsetMats[aBoneIDs[0]]  * aWeights[0];
//    offsetTransformation += bonesOffsetMats[aBoneIDs[1]] * aWeights[1];
//    offsetTransformation += bonesOffsetMats[aBoneIDs[2]] * aWeights[2];
//    offsetTransformation += bonesOffsetMats[aBoneIDs[3]] * aWeights[3];

    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;

//    vec4 tmp = offsetTransformation * vec4(WorldPos, 1.0);
//    gl_Position = projection * view * bone_transformation * tmp;
//    gl_Position = bone_transformation * projection * view * offsetTransformation * vec4(WorldPos, 1.0);
    gl_Position = bone_transformation * projection * view * vec4(aPos, 1.0);
//    gl_Position = projection * view * vec4(WorldPos, 1.0);
}