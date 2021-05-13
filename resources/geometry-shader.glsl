#version 150

uniform mat4 projection;  // Aspect ratio stretches primitives

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void main() {
    gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.1,  0.1, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4( 0.1,  0.1, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.1, -0.1, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4( 0.1, -0.1, 0.0, 0.0));
    EmitVertex();

    EndPrimitive();
}