// Fragment shader: Wave shading
// ================
#version 450 core

in vec3 Color;

// Ouput data
out vec4 FragColor;

void main()
{
   FragColor = vec4(Color, 1.0);
}
