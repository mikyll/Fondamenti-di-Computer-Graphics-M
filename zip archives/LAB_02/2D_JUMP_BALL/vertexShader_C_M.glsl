#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1
  
out vec4 ourColor; // output a color to the fragment shader

//VARIABILE DI TIPO uniform, rimane la stessa per ogni vertice della primitiva e viene passata dall'esterno
uniform mat4 Projection;  
uniform mat4 Model;

void main()
{
// Trasformazione dei vertici dalle coordinate nel sistema di riferimento dell'oggetto (apos), 
// al sistema di riferimento del mondo (premoltiplicazione per matrice Model) 

    gl_Position = Projection*Model*vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}  

