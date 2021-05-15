#version 150

uniform mat4 projection;  // Aspect ratio stretches primitives
// uniform mat4 camera;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 geoColor[];
out vec4 fragColor;

void main() {
    fragColor = geoColor[0];
    
    gl_Position = gl_in[0].gl_Position + projection * vec4(-0.01,  0.01, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + projection * vec4( 0.01,  0.01, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + projection * vec4(-0.01, -0.01, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + projection * vec4( 0.01, -0.01, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();

}