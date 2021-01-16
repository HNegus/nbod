#version 330 core

layout (lines) in;
layout (line_strip, max_vertices = 6) out;

void main (void) {
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec4 d1 = p1 + vec4(p1.x * 0.1, p2.y + p2.y * 0.1, 0, 0);
    vec4 d2 = p2 + vec4(p2.x * 0.1, p2.y * p2.y + 0.1, 0, 0);


    gl_Position = p1;
    EmitVertex();
    // gl_Position = d1;
    // EmitVertex();
    gl_Position = p2;
    EmitVertex();

    // gl_Position = d1;
    // EmitVertex();
    // gl_Position = p2;
    // EmitVertex();
    // gl_Position = d2;
    // EmitVertex();

    // gl_Position = gl_in[1].gl_Position + pos;
    // EmitVertex();

    EndPrimitive();
}
