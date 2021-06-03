#version 150

// uniform mat4 model;
uniform mat4 projection;  // Aspect ratio
uniform mat4 camera;

in vec2 vert;
in vec4 vertColor;
out vec4 fragColor;
// in vec2 vertTexCoord;
// out vec2 fragTexCoord;

void main() {
    fragColor = vertColor;
    // gl_Position = projection * camera * vec4(vert, 0, 1);
    gl_Position = projection * camera * vec4(vert, 0, 1);
    // gl_Position = mat4(1.0) * vec4(vert, 0, 1);
}
