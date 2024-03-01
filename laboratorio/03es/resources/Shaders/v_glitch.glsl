#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling

out vec2 vUv;

void main() {
    vUv = uv;
    gl_Position = P * V * M * vec4(position, 1.0);
}