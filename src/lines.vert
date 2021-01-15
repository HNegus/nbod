#version 330 core

layout(location = 0) in vec4 position;
// layout(location = 1) in vec4 inColor;
// layout(location = 2) in vec2 inCenter;

// out vec4 outColor;

//
uniform mat4 u_MVP;

void main()
{
    gl_Position = position;
    gl_Position = u_MVP * position;


    // outColor = inColor;
}
