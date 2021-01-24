#version 330 core

// layout(location = 0) in vec4 color;

in vec4 outColor;



void main()
{

    // gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    gl_FragColor = outColor;

};
