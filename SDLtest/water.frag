#version 400 core

out vec4 color;

in vec4 position0;
in vec2 worldPosition0;
in vec3 toEye0;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;
uniform sampler2D reflexion;
uniform sampler2D dudvMap;
uniform sampler2D gPosition;

uniform float moveFactor;

float LinearizeDepth(float depth) ;

void main() {
	vec2 d1 = texture(dudvMap, vec2(worldPosition0.x + moveFactor, worldPosition0.y)).rg *2 -1;
	vec2 d2 = texture(dudvMap, vec2(-worldPosition0.x, worldPosition0.y + moveFactor)).rg *2 -1;
	vec2 offset = d1 + d2;

	vec3 n1 = texture(textureNormal, vec2(worldPosition0.x + moveFactor, worldPosition0.y)).rgb *2 -1;
	vec3 n2 = texture(textureNormal, vec2(-worldPosition0.x, worldPosition0.y + moveFactor)).rgb *2 -1;
	vec3 normal = normalize(n1 + n2);
	vec2 texCoord = position0.xy / position0.w /2 + 0.5 + offset * 0.01;

	vec3 toLight = normalize(vec3(.6, 1, .7));
	vec3 halfwayDir = normalize(toLight + toEye0);
	float spec = pow(max(dot(normal, halfwayDir), 0), 20);
	vec3 specular = 1 * spec * vec3(.7, .8, .9);

	float reflectivity = 1 - dot(normalize(toEye0), vec3(0, 0, 1));
	float transparency = texture(gPosition, texCoord).a  - LinearizeDepth(gl_FragCoord.z);
	transparency = clamp(transparency * 20, 0, 1);

	color.rgb = mix(texture(textureDiffuse, texCoord), texture(reflexion, vec2(1 - texCoord.x, texCoord.y)), reflectivity).rgb + specular * .05;
	color.a = transparency;
}

float LinearizeDepth(float depth) 
{
	float near = .1; 
	float far  = 100.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}