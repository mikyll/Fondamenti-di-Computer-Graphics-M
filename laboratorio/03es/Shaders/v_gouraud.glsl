// Vertex shader: Gouraud shading
// ================
#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Color; // resulting color from lighting calculations

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

// Vectors
// "Eye" vector (from camera to the vertex to be shaded). 
// Helps determining how much of the surface is visible from 
// the viewer's perspective.
vec3 E;

// "Normal" vector (surface normal at the vertex to be shaded). 
// Used to determine how light interacts with the surface, 
// and also needed to calculate the angle between the incoming 
// light and the surface.
vec3 N;

// "Light" vector (direction from the surface point, to the light
// source).
// Helps determining how much light illuminates the surface, this
// one too is needed to calculate the angle between the incoming 
// light and the surface.
vec3 L;

// "Reflection" vector (direction of the perfect reflection of the
// light vector L about the surface normal N).
// Fundamental for determining the specular highlight on a surface,
// contributes to the shiny appearance of materials.
vec3 R;

void main()
{
    gl_Position = P * V * M * vec4(aPos, 1.0);
    
    // Transform vertex position into eye (VCS) coordinates
    vec4 eyePosition = V * M * vec4(aPos, 1.0);
	// Transform Light  position into eye (VCS) coordinates 
	vec4 eyeLightPos = V * vec4(light.position, 1.0);
    // Transform vertex normal into VCS
    N = normalize(transpose(inverse(mat3(V * M))) * aNormal);

	// Compute vectors E,L,R in VCS
	E = normalize(-eyePosition.xyz);
	L = normalize((eyeLightPos - eyePosition).xyz);
    R = reflect(-L, N);  

    // ambient
    vec3 ambient = light.power * material.ambient;
  	
    // diffuse 
    float diff = max(dot(L,N), 0.0);
    vec3 diffuse = light.power * light.color * diff * material.diffuse;

    // specular
    float spec = pow(max(dot(E, R), 0.0), material.shininess);
    vec3 specular =  light.power * light.color * spec * material.specular;  

    Color = ambient + diffuse + specular;
}