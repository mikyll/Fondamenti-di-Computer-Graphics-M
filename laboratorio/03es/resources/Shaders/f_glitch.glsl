#version 450 core

out vec4 fragColor;
in vec2 vUv;

uniform float time;
uniform vec2 resolution;

// Simplex noise function or a similar noise function is required
// Placeholder for noise function
float noise(vec2 p) {
    return fract(sin(dot(p.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // Basic glitch effect using noise
    vec2 pos = vUv + vec2(noise(vUv + time * 0.1) * 0.005 - 0.0025, 0.0);
    float n = noise(pos * time);
    
    // Simple color manipulation
    vec3 color = mix(vec3(0.9, 0.2, 0.3), vec3(0.1, 0.1, 0.1), n);
    
    fragColor = vec4(color, 1.0);
}
