#version 330 core

in vec3 in_Vertex;
in vec2 in_TexCoord;

out vec2 texCoord0;

void main()
{
    gl_Position = vec4(in_Vertex, 1.0f); 
    texCoord0 = in_TexCoord;
} 