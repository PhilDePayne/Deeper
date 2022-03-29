#TODO: recursive add to source files
set(SOURCE_FILES
	include/Component.h
	src/Component.cpp
    include/CubeMesh.h
	src/CubeMesh.cpp
 "include/GameObject.h" "src/GameObject.cpp" "include/typedefs.h" "include/Shader.h" "include/Camera.h")

add_library(Biblioteka ${SOURCE_FILES})

target_include_directories(Biblioteka PUBLIC include)