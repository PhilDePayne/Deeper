#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec2 aTex;

uniform mat4 model;
uniform mat4 proj;

// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;


void main()
{
    // Outputs the positions/coordinates of all vertices
    gl_Position = proj * model *  vec4(aPos, 1.0);
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
    texCoord = aTex;
}