//
// Created by Wojtek on 06.05.2022.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "Model.h"
#include "glm/gtx/string_cast.hpp"
#include <stb_image.h>
#include "LogMacros.h"

// Log macros to disable/enable certain logs in model class
#ifdef DEEPER_MODEL_CLASS_LOGS

#define DRAW_NODE_TREE_LOG
//#define TEXTURE_LOAD_LOG
#define PROCESS_MESH_LOG
//#define COLLIDER_COORD_LOG
#define MODEL_GENERAL_INFO_LOG

#endif

// draws matrix in console for debugging
void drawAiMatrix(aiMatrix4x4 matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}

void drawGLMMatrix(glm::mat4 matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << matrix[j][i] << ", ";
        }
        std::cout << std::endl;
    }
}

Model::Model(char *path)
{
    loadModel(path);
}

bool isOnOrForwardPlan(BoxCollider b, Plan p, glm::mat4 proj, glm::mat4 view) {

    BoxCollider tmp;

    float divisor;

    tmp.setCenter(proj * view * glm::vec4(b.getCenter(), 1.0f));
    glm::vec3 tmpMaxPoints = proj * view * glm::vec4(b.getMaxX(), b.getMaxY(), b.getMaxZ(), 1.0f);
    tmp.setSize(glm::vec3((tmpMaxPoints.x - tmp.getCenter().x) * 2, (tmpMaxPoints.y - tmp.getCenter().y) * 2, (tmpMaxPoints.z - tmp.getCenter().z) * 2));

    divisor = tmp.getCenter().y / b.getCenter().y;

    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    float r = tmp.getSizeX() / 2 * glm::abs(p.normal.x) + tmp.getSizeY() / 2 * glm::abs(p.normal.y) + tmp.getSizeZ() / 2 * glm::abs(p.normal.z);

    // Compute distance of box center from plane
    float s = glm::dot(p.normal, tmp.getCenter()) - p.distance;

    // Intersection occurs when distance s falls within [-r,+r] interval
    return -r <= p.getSignedDistanceToPlan(tmp.getCenter());

}

void Model::Draw(Shader shader)
{
//    std::vector<glm::mat4> convertedBonesMatrices(bonesTransforms.size());
//    shader.use();
//    for (int i = 0; i < bonesTransforms.size(); i++)
//    {
//        glm::mat4 convertedBoneMatrix;
//        aiToGlmTransformMatrix(bonesTransforms[i], convertedBoneMatrix);
//        shader.setMat4("bones[" + std::to_string(i) + "]", convertedBoneMatrix);
//        drawGLMMatrix(convertedBoneMatrix);
//    }

//    for (int i = 0; i < offsetMatrices.size(); i++)
//    {
//        glm::mat4 convertedOffsetMatrix;
//        aiToGlmTransformMatrix(offsetMatrices[i], convertedOffsetMatrix);
//        shader.setMat4("bonesOffsetMats[" + std::to_string(i) + "]", convertedOffsetMatrix);
//    }

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
            if (meshes[i].getTexturesSetId() != loadedSet)
            {
                passMapsToShader(meshes[i].getTexturesSetId());
            }

            meshes[i].Draw(shader, transform);
        
    }
}

void Model::Draw(Shader shader, Frustum& frustum, glm::mat4 &proj, glm::mat4 &view, int &renderCount)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        BoxCollider tmpBV;

        tmpBV.setCenter(meshes[i].boundingVolume.getCenter() * transform.scale);
        tmpBV.setSize(glm::vec3(meshes[i].boundingVolume.getSizeX(), meshes[i].boundingVolume.getSizeY(), meshes[i].boundingVolume.getSizeZ()) 
            * transform.scale);

        tmpBV.setCenter(glm::translate(glm::mat4(1.0f), transform.position) * glm::vec4(tmpBV.getCenter(), 1.0f));

        if (isOnOrForwardPlan(tmpBV, frustum.topFace, proj, view) &&
            isOnOrForwardPlan(tmpBV, frustum.bottomFace, proj, view)) {

            renderCount++;

            if (meshes[i].getTexturesSetId() != loadedSet)
            {
                passMapsToShader(meshes[i].getTexturesSetId());
            }

            meshes[i].Draw(shader, transform);
        }

    }
}

void Model::loadModel(std::string path)
{
#ifdef MODEL_GENERAL_INFO_LOG
    printf("\nLoading model at path: %s \n", path.c_str());
#endif

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

#ifdef MODEL_GENERAL_INFO_LOG
    printf("Meshes: %u, Animations: %u \n", scene->mNumMeshes, scene->mNumAnimations);
    if (scene->mNumAnimations != 0)
    {
        for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
        {
            std::cout << "channel " << scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << std::endl;
        }
    }
#endif

    // process ASSIMP's root node recursively
    aiMatrix4x4 startMatrix;
    processNode(scene->mRootNode, scene, &startMatrix, 0);

    // after processing all nodes, match bones with corresponding nodes transformations
//    bonesTransforms.resize(boneToId.size());
//    offsetMatrices.resize(boneToId.size());
//    for (auto& boneAndId : boneToId)
//    {
//        bonesTransforms[boneAndId.second] = nodeNameToTransform[boneAndId.first];
//        offsetMatrices[boneAndId.second] = bonesToOffsetMatrices[boneAndId.first];
//    }

#ifdef MODEL_GENERAL_INFO_LOG
    printf("Total nodes in model: %u\n", totalNodes);
//    printf("BoneToId size: %llu\n", boneToId.size());
//    printf("nodeNameToTransform size: %llu\n", nodeNameToTransform.size());
//    printf("bonesToOffsetMatrices size: %llu\n", bonesToOffsetMatrices.size());
#endif
}

// variable depth if only for debugging and printing simple node tree in console
void Model::processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 *transformMatrix, GLuint depth)
{
#ifdef DRAW_NODE_TREE_LOG
    std::string depthPrefix;
    for (int i = 0; i < depth; i++) depthPrefix.append("-");
    printf("%sNode_name: %s, meshes: %u\n", depthPrefix.c_str(), node->mName.C_Str(), node->mNumMeshes);
    depth++;
#endif
#ifdef MODEL_GENERAL_INFO_LOG
    totalNodes++;
#endif

    // multiply given matrix with transform matrix of the current node
    aiMatrix4x4 currentTransform = *transformMatrix * node->mTransformation;

    //Ten if raczej jest niepotrzebny
//    if(nodeNameToTransform.find(node->mName.C_Str()) == nodeNameToTransform.end())
//    {
//        nodeNameToTransform[node->mName.C_Str()] = currentTransform;
//    }
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, currentTransform));
    }

    // process all children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, &currentTransform, depth);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 transformMatrix)
{
#ifdef PROCESS_MESH_LOG
    printf("Mesh_name: %s, num_bones: %u, numVertices: %u\n", mesh->mName.C_Str(), mesh->mNumBones, mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumBones; ++i)
    {
        printf("bone%u_name: %s, numWeights: %u\n", i,
               mesh->mBones[i]->mName.C_Str(), mesh->mBones[i]->mNumWeights);
//        printf("--Weight1 Id: %u, weight: %f\n", mesh->mBones[i]->mWeights[0].mVertexId, mesh->mBones[i]->mWeights[0].mWeight);
//        printf("--Weight2 Id: %u, weight: %f\n", mesh->mBones[i]->mWeights[1].mVertexId, mesh->mBones[i]->mWeights[1].mWeight);
    }
#endif


    // Check if the texture with given prefix is already loaded. If it is - don't load again, if it's not - load.
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    // Texture loading
    std::string texturePrefix = mesh->mName.C_Str();
    texturePrefix = texturePrefix.substr(0, texturePrefix.find_first_of('_'));
    bool textureNotLoaded = true;
    GLuint textureSetId;
    for (const MapsSet& set : mapsSets_loaded)
    {
        if (set.PrefixName == texturePrefix)
        {
            textureSetId = set.Id;
            textureNotLoaded = false;
            break;
        }
    }
    if (textureNotLoaded)
    {
        textureSetId = loadMapsSet(texturePrefix);
    }

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly
        // convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
//            vector.x = mesh->mTangents[i].x;
//            vector.y = mesh->mTangents[i].y;
//            vector.z = mesh->mTangents[i].z;
//            vertex.Tangent = vector;
            // bitangent
//            vector.x = mesh->mBitangents[i].x;
//            vector.y = mesh->mBitangents[i].y;
//            vector.z = mesh->mBitangents[i].z;
//            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        setVertexBoneDataDefault(vertex);

        vertices.push_back(vertex);
    }

    // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

//    for(unsigned int i = 0; i < mesh->mNumBones; i++)
//    {
//        aiBone *tmpBone = mesh->mBones[i];
//        bonesToOffsetMatrices[std::string(tmpBone->mName.C_Str())] = tmpBone->mOffsetMatrix;
//        printf("\nOffset matrix %u\n", i);
//        drawAiMatrix(tmpBone->mOffsetMatrix);
//
//        for(unsigned int j = 0; j < tmpBone->mNumWeights; j++)
//        {
//            if(j >= MAX_BONE_INFLUENCE) break;
//            std::string boneName(tmpBone->mName.C_Str());
//            int boneID = 0;
//            if (boneToId.find(boneName) == boneToId.end())
//            {
//                boneID = (int)boneToId.size();
//                boneToId[boneName] = boneID;
//            }
//            else
//            {
//                boneID = (int)boneToId[boneName];
//            }
//            vertices[tmpBone->mWeights[j].mVertexId].m_BoneIDs[j] = boneID;
//            vertices[tmpBone->mWeights[j].mVertexId].m_Weights[j] = tmpBone->mWeights[j].mWeight;
//        }
//    }

    glm::mat4 convertedMatrix(1.0f);

    // Matrices from assimp and glm have different indexing, so they have to be converted
    aiToGlmTransformMatrix(transformMatrix, convertedMatrix);

//    printf("ASSIMP MAT\n");
//    drawAiMatrix(transformMatrix);
    //printf("GLM MAT\n");
    //std::cout << glm::to_string(convertedMatrix) << '\n';

    extractBoneWeightForVertices(vertices, mesh);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, convertedMatrix, textureSetId);
}



GLuint Model::loadMapsSet(const std::string &prefix)
{
    MapsSet mapsSet;
    GLuint Id = mapsSets_loaded.size() + 1;
    mapsSet.Id = Id;
    mapsSet.PrefixName = prefix;
    mapsSet.Albedo = textureFromFile(prefix, "Albedo");
    mapsSet.Normal = textureFromFile(prefix, "Normal");
    mapsSet.Metallic = textureFromFile(prefix, "Metallic");
    mapsSet.Roughness = textureFromFile(prefix, "Roughness");
    mapsSet.AO = textureFromFile(prefix, "AO");
    mapsSet.Emissive = textureFromFile(prefix, "Emissive");
    mapsSets_loaded.push_back(mapsSet);
    return Id;
}

unsigned int Model::textureFromFile(const std::string &prefix, const std::string &type)
{
    std::string filePath = directory + "/" + prefix + "_" + type + ".png";
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
#ifdef TEXTURE_LOAD_LOG
        std::cout << "Texture loaded at path: " << filePath << std::endl;
#endif
    }
    else
    {
#ifdef TEXTURE_LOAD_LOG
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
#endif
        stbi_image_free(data);
    }

    return textureID;
}

void Model::passMapsToShader(GLuint mapsId)
{
    for (const MapsSet &set : mapsSets_loaded)
    {
        if (set.Id == mapsId)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, set.Albedo);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, set.Normal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, set.Metallic);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, set.Roughness);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, set.AO);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, set.Emissive);
            break;
        }
    }
}

std::vector<BoxCollider> Model::getColliders()
{

    std::vector<BoxCollider> ret;

    BoxCollider tmp;

    tmp.setCenter(glm::vec3(0.0f));
    tmp.setSize(glm::vec3(0.0f));

    for(auto i : meshes){
    
        tmp.setSize(glm::vec3(i.boundingVolume.getSizeX(), i.boundingVolume.getSizeY(), i.boundingVolume.getSizeZ()) *
            transform.scale);

        tmp.setCenter(glm::vec3(glm::translate(glm::mat4(1.0f), transform.position) * glm::vec4(i.boundingVolume.getCenter() * transform.scale, 1.0f)));

#ifdef COLLIDER_COORD_LOG
        printf("%f %f %f %f %f %f\n", tmp.getCenter().x, tmp.getCenter().y, tmp.getCenter().z, tmp.getSizeX(), tmp.getSizeY(), tmp.getSizeZ());
#endif
        ret.push_back(tmp);

    }

    return ret;

}

void Model::aiToGlmTransformMatrix(aiMatrix4x4 &source, glm::mat4 &target)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            target[i][j] = source[j][i];
        }
    }
}

void Model::setVertexBoneDataDefault(Vertex &vertex)
{
    for (GLuint i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}

void Model::setVertexBoneData(Vertex &vertex, int boneId, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneId;
            break;
        }
    }
}

void Model::extractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (BoneInfoMap.find(boneName) == BoneInfoMap.end())
        {
            BoneInfo newBoneInfo{};
            newBoneInfo.Id = BoneCounter;
            glm::mat4 offsetMatrix(0);
            aiToGlmTransformMatrix(mesh->mBones[boneIndex]->mOffsetMatrix, offsetMatrix);
            newBoneInfo.offsetMatrix = offsetMatrix;
            BoneInfoMap[boneName] = newBoneInfo;
            boneID = BoneCounter;
            BoneCounter++;
        }
        else
        {
            boneID = BoneInfoMap[boneName].Id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            unsigned int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            setVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

std::map<std::string, BoneInfo> &Model::GetBoneInfoMap()
{
    return BoneInfoMap;
}

int &Model::GetBoneCount()
{
    return BoneCounter;
}
