// Vertex shader: Toon shading
// ================
#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // = position * rotation * scaling

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

// Ouput data
out vec3 E;
out vec3 N;
out vec3 L;

void main()
{
    gl_Position = P * V * M * vec4(aPos, 1.0);
	
	// Transform vertex  position into eye (VCS) coordinates
    vec4 eyePosition = V * M * vec4(aPos, 1.0);
	// Transform Light position into eye (VCS) coordinates 
	vec4 eyeLightPos = V * vec4(light.position, 1.0);


    // Transform vertex normal into VCS
    N = normalize(transpose(inverse(mat3(V * M))) * aNormal);
	
	// Compute vectors E, L in VCS
	E = normalize(-eyePosition.xyz);
	L = normalize((eyeLightPos - eyePosition).xyz);
}