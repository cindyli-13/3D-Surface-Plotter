#version 460 core

in vec3 colorPos;
in vec3 fragPos;

uniform float zRange;

void main() {
    gl_FragColor = vec4(fragPos.z/zRange, fragPos.z/zRange, fragPos.z/zRange, 1.0);
}
