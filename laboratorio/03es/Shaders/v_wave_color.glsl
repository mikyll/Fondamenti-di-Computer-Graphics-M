// Vertex shader: Wave shading
// ================
#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Resulting color from lighting calculations
out vec3 Color; 

// Projection, View, Model matrices
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // = position * rotation * scaling

uniform float time; // in milliseconds

uniform float xFreq = 0.001, zFreq = 0.001;
uniform float heightScale = 0.1;

void main()
{
	vec4 v = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	
	// Update the Y component (wave height)
	v.y = aPos.y + 
	heightScale * sin(time * xFreq + 5 * aPos.x) +
	heightScale * sin(time * zFreq + 5 * aPos.z);
	
	gl_Position = P * V * M * v;
	
	Color = vec3(v.x, v.y * 5, v.z);
}