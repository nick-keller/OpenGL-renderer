#version 400 core

in vec3 in_Vertex;
in vec3 in_Normal;

out vec3 normal;
out vec3 toLight;
out vec3 toCamera;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 worldPosition = model * vec4(in_Vertex, 1.0);

	gl_Position = projection * view * worldPosition;
	normal = (model * vec4(in_Normal, 1.0)).xyz;
	toLight = (vec4(-1, 0, 0, 1.0)).xyz;
	toCamera = (inverse(view) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;
}