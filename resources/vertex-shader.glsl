#version 150

// uniform mat4 model;
uniform mat4 projection;  // Aspect ratio affects the position of the primitives

in vec2 vert;
in vec2 vertTexCoord;
// out vec2 fragTexCoord;

void main() {
    // fragTexCoord = vertTexCoord;
    // gl_Position = vec4(vert, 1) + projection * vec4(0, 0, 0, 0);
    gl_Position = projection * vec4(vert, 0, 1);
    // gl_Position = vec4(vert, 0, 1);
}
