#version 460 core

in vec3 fragPos;

out vec4 FragColor;

uniform float zMin;
uniform float zRange;

void main() {
    float contrast = 0.6;
    FragColor = vec4(((fragPos.z-zMin)/zRange - 0.5) * contrast + 0.7, ((fragPos.z-zMin)/zRange - 0.5) * contrast + 0.7, ((fragPos.z-zMin)/zRange - 0.5) * contrast + 0.7, 1.0);
}
