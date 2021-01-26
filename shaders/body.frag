#version 330 core

layout(location = 0) out vec4 color;

in float outRadius;
in vec2 outCenter;
in vec2 outPosition;
in vec4 outColor;


void main()
{
    float radius = 0.9 * outRadius;
    float border = 0.01 * radius;
    vec2 v = abs(outCenter - outPosition);
    if (length(v) > radius + border) {
        color = vec4(0.0, 0.0, 0.0, 0.0);
    } else {
        color = outColor;
    }
    return;

};
