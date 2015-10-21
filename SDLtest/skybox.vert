#version 400 core

in vec3 in_Vertex;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
    gl_Position = projection * view * vec4(in_Vertex, 1.0);  
    TexCoords = vec3(in_Vertex.x, in_Vertex.z, in_Vertex.y);
} 