#version 150
// uniform sampler2D tex;

uniform vec4 color;

// in vec2 fragTexCoord;
out vec4 finalColor;

void main() {
    finalColor = color; // * texture(tex, fragTexCoord);
}
