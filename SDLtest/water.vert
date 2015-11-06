#version 400 core

in vec3 in_Vertex;

out vec4 position0;
out vec2 worldPosition0;
out vec3 toEye0;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 screenPosition = projection * view * vec4(in_Vertex, 1);

	gl_Position = screenPosition;

	position0 = screenPosition;
	worldPosition0 = in_Vertex.xy;
	toEye0 = (inverse(view) * vec4(0, 0, 0, 1)).xyz - in_Vertex;
}