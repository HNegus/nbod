#version 330 core

// layout(location = 0) in vec4 color;

// in float outRadius;
in vec4 outColor;
// in vec2 outPosition;
// in vec2 v_TexCoord;
// in vec4 baseColor;

// uniform vec4 u_Color;
// uniform vec4 m_RGBA;
// uniform sampler2D u_Texture;


void main()
{
    // vec4 texColor = texture(u_Texture, v_TexCoord);
    // vec4 compositeColor = mix(u_Color, 1 -  m_RGBA, 0.5);
    // color = texColor * baseColor + compositeColor;
    // float radius = 0.9 * outRadius;
    // float border = 0.01 * radius;
    // vec2 v = abs(outCenter - outPosition);
    // if (length(v) > radius + border) {
        // color = vec4(0.0, 0.0, 0.0, 0.0);
    // } else if (length(v)  > radius) {
        // float difference = (length(v) - radius) / (radius + border);
        // color = vec4(0.0, 1.0, 1.0, 1.0);
    // } else {
        // color = vec4(0.0, 1.0, 0.0, 1.0);
    // }
    // return;
    // color = vec4(0.0, 1.0, 0.0, 1.0);
    // gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    gl_FragColor = outColor;


    // color = texColor;
    // color = u_Color;

};
