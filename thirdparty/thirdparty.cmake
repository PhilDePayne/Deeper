# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARY ${OPENGL_LIBRARIES})

# assimp
find_library(ASSIMP_LIBRARY "assimp" "/usr/lib" "/usr/local/lib")
find_path(ASSIMP_INCLUDE_DIR "assimp/mesh.h" "/usr/include" "/usr/local/include")

if((NOT ASSIMP_LIBRARY) OR (NOT ASSIMP_INCLUDE_DIR))
	set(ASSIMP_DIR "${THIRDPARTY_DIR}/assimp")

	message("Unable to find assimp, cloning...")
	execute_process(COMMAND git submodule update --init ${ASSIMP_DIR}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build package with shared libraries.") #tu (on - freetype, off - nie)
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "If the supplementary tools for Assimp are built in addition to the library.")
	set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")

	add_subdirectory("${ASSIMP_DIR}")

	set(ASSIMP_LIBRARY "assimp")
	set(ASSIMP_INCLUDE_DIR "${ASSIMP_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/assimp/include")
endif()

set(CMAKE_DEBUG_POSTFIX "")

# glfw
find_library(GLFW_LIBRARY "glfw" "/usr/lib" "/usr/local/lib")
find_path(GLFW_INCLUDE_DIR "glfw/glfw.h" "/usr/include" "/usr/local/include")

if((NOT GLFW_LIBRARY) OR (NOT GLFW_INCLUDE_DIR))
	set(GLFW_DIR "${THIRDPARTY_DIR}/glfw")

	message("Unable to find glfw, cloning...")
	execute_process(COMMAND git submodule update --init ${GLFW_DIR}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
	set(GLFW_INSTALL        OFF CACHE INTERNAL "Generate installation target")

	add_subdirectory("${GLFW_DIR}")

	set(GLFW_LIBRARY "glfw" "${GLFW_LIBRARIES}")
	set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
endif()

# glad
set(GLAD_DIR "${THIRDPARTY_DIR}/glad")
add_library("glad" STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

set(GLAD_LIBRARY "glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

# glm
set(GLM_DIR "${THIRDPARTY_DIR}/glm")
execute_process(COMMAND git submodule update --init ${GLM_DIR}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(GLM_INCLUDE_DIR "${GLM_DIR}")

# imgui
set(IMGUI_DIR "${THIRDPARTY_DIR}/imgui")
execute_process(COMMAND git submodule update --init ${IMGUI_DIR}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library("imgui" STATIC "${IMGUI_DIR}/imgui.cpp"
		"${IMGUI_DIR}/imgui_demo.cpp"
		"${IMGUI_DIR}/imgui_draw.cpp"
		"${IMGUI_DIR}/imgui_tables.cpp"
		"${IMGUI_DIR}/imgui_widgets.cpp")
target_include_directories("imgui" PRIVATE "${IMGUI_DIR}")

set(IMGUI_LIBRARY "imgui")
set(IMGUI_INCLUDE_DIR "${IMGUI_DIR}")

# stb_image
set(STB_IMAGE_DIR "${THIRDPARTY_DIR}/stb_image")
add_library("stb_image" STATIC "${STB_IMAGE_DIR}/stb_image.cpp")
target_include_directories("stb_image" PRIVATE "${STB_IMAGE_DIR}")

set(STB_IMAGE_LIBRARY "stb_image")
set(STB_IMAGE_INCLUDE_DIR "${STB_IMAGE_DIR}")

# fmod
set(FMOD_DIR "${THIRDPARTY_DIR}/fmod")
add_library("fmod" SHARED IMPORTED)

set_property(TARGET "fmod" PROPERTY
		IMPORTED_LOCATION "${FMOD_DIR}/lib/fmod.dll")
set_property(TARGET "fmod" PROPERTY
		IMPORTED_IMPLIB "${FMOD_DIR}/lib/fmod_vc.lib")
set_property(TARGET "fmod" PROPERTY
		INTERFACE_INCLUDE_DIRECTORIES "${FMOD_DIR}/inc")

set(FMOD_LIBRARY "fmod")
set(FMOD_INCLUDE_DIR "${FMOD_DIR}")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/src")
file(COPY "${FMOD_DIR}/lib/fmod.dll" DESTINATION "${EXECUTABLE_OUTPUT_PATH}")

# freetype
find_library(FREETYPE_LIBRARY "freetype" "/usr/lib" "/usr/local/lib")
find_path(FREETYPE_INCLUDE_DIR "freetype/ft2build.h" "/usr/include" "/usr/local/include")

if((NOT FREETYPE_LIBRARY) OR (NOT FREETYPE_INCLUDE_DIR))
	set(FREETYPE_DIR "${THIRDPARTY_DIR}/freetype")

	message("Unable to find freetype, cloning...")
	execute_process(COMMAND git submodule update --init ${FREETYPE_DIR}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	#	set(FT_DISABLE_ZLIB ON)

	add_subdirectory("${FREETYPE_DIR}")

	set(FREETYPE_LIBRARY "freetype" "${FREETYPE_LIBRARIES}")
	set(FREETYPE_INCLUDE_DIR "${FREETYPE_DIR}/include")
endif()

#kopiowanie dllek
file(COPY "${THIRDPARTY_DIR}/dlls/glfw3.dll" DESTINATION "${EXECUTABLE_OUTPUT_PATH}")
file(COPY "${THIRDPARTY_DIR}/dlls/assimp-vc142-mtd.dll" DESTINATION "${EXECUTABLE_OUTPUT_PATH}")
file(COPY "${THIRDPARTY_DIR}/dlls/freetyped.dll" DESTINATION "${EXECUTABLE_OUTPUT_PATH}")