#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in float inRadius;
layout(location = 2) in vec2 inCenter;

out float outRadius;
out vec2 outCenter;
out vec2 outPosition;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    // gl_Position = position;
    // v_TexCoord = texCoord;
    // baseColor = inColor;
    outPosition = position.xy;
    outRadius = inRadius;
    outCenter = inCenter;
}
