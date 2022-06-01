#TODO: recursive add to source files
set(SOURCE_FILES
	include/Component.h
	src/Component.cpp
    include/CubeMesh.h
	src/CubeMesh.cpp
 	"include/GameObject.h"
 	"src/GameObject.cpp"
 	"include/typedefs.h"
 	"include/Shader.h"
 	"include/Camera.h"
 	"include/Texture.h"
 	"src/Texture.cpp"
 	"include/PlainMesh.h"
 	"src/PlainMesh.cpp"
 	"include/SceneGraphNode.h"
 	"src/SceneGraphNode.cpp"
 	"include/Transform.h"
 	"src/Transform.cpp"
 	"include/SphereMesh.h"
 	"src/SphereMesh.cpp"
 	"include/Collider.h"
 	"src/Collider.cpp"
 	"include/SphereCollider.h"
 	"src/SphereCollider.cpp"
 	"include/BoxCollider.h"
 	"src/BoxCollider.cpp"
 	"include/LightSource.h"
 	"src/LightSource.cpp"
 	include/Model.h
	src/Model.cpp
	include/Mesh.h
    src/Mesh.cpp
    include/Frustum.h
	include/Player.h
	include/LogMacros.h
		include/Bone.h
		src/Bone.cpp
		include/AssimpGLMHelpers.h
		src/AssimpGLMHelpers.cpp
		include/Animation.h
		src/Animation.cpp
		include/Animator.h
		src/Animator.cpp)

#TODO: Biblioteka variable
add_library(Biblioteka ${SOURCE_FILES})

target_include_directories(Biblioteka PUBLIC include)

target_include_directories(Biblioteka PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Biblioteka PUBLIC "${ASSIMP_INCLUDE_DIR}")
target_include_directories(Biblioteka PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(Biblioteka PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(Biblioteka PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(Biblioteka PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(Biblioteka PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(Biblioteka PUBLIC "${FMOD_INCLUDE_DIR}")

target_link_libraries(Biblioteka "${OPENGL_LIBRARY}")
target_link_libraries(Biblioteka "${ASSIMP_LIBRARY}")
target_link_libraries(Biblioteka "${GLFW_LIBRARY}")
target_link_libraries(Biblioteka "${GLAD_LIBRARY}"      "${CMAKE_DL_LIBS}")
target_link_libraries(Biblioteka "${IMGUI_LIBRARY}"     "${CMAKE_DL_LIBS}")
target_link_libraries(Biblioteka "${STB_IMAGE_LIBRARY}" "${CMAKE_DL_LIBS}")
target_link_libraries(Biblioteka "${FMOD_LIBRARY}")