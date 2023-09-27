// Fragment shader: Toon shading
// ================
#version 450 core

const float a = 0.8;
const float b = 0.6;
const float c = 0.5;
const float d = 0.6;
const float e = 0.1;

in vec3 Color;

// Input data
in vec3 N, E, L;

// Ouput data
out vec4 fragColor;

void main()
{
	vec4 color = vec4(Color, 1.0);
	float intensity = dot(normalize(L),normalize(N));
	
	if (intensity > 0.95)
	{
		color *= a;
	}
	else if (intensity > 0.5)
	{
		color *= b;
	}
	else if (intensity > 0.25)
	{
		color *= c;
	}
	else if (intensity > 0.15)
	{
		color *= d;
	}
	else
	{
		color *= e;
	}
	float aa = dot(normalize(E), normalize(N));
	
	if(aa >= 0.0 && aa < 0.30)
	{
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	
	fragColor = color;
}