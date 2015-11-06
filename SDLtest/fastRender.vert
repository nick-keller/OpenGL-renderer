#version 400 core

in vec3 in_Vertex;
in vec2 in_TexCoord;
in vec3 in_Normal;

out vec3 position0;
out vec3 normal0;
out vec2 texCoord0;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 worldPosition = model * vec4(in_Vertex, 1);
	gl_ClipDistance[0] = worldPosition.z + .05;

	gl_Position = projection * view * worldPosition;

	position0 = worldPosition.xyz;
	texCoord0 = in_TexCoord;
	
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	normal0 = normalMatrix * in_Normal;
}