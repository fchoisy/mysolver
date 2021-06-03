#version 150
// uniform sampler2D tex;

// uniform vec4 color;

in vec4 fragColor;
out vec4 finalColor;

void main() {
    // finalColor = fragColor * vec4(0, 0, 0, 0) + vec4(1,1,1,1); // * texture(tex, fragTexCoord);
    finalColor = fragColor; // * texture(tex, fragTexCoord);
}
