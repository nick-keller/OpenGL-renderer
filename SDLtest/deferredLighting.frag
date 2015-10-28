#version 330 core

out vec4 FragColor;

in vec2 texCoord0;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssaoLevel;
uniform mat4 view;

struct PointLight {
    vec3 position;  
    vec3 color;
	float attenuation;
};

struct SpotLight {
    vec3 position;  
    vec3 color;
	vec3 direction;
	float spread;
	float outerSpread;
	float attenuation;
};

struct AmbiantLight {
    vec3 color;
	float strength;
};

vec3 computeLighting(PointLight light, vec3 position, vec3 normal, vec3 toEye);
vec3 computeLighting(SpotLight light, vec3 position, vec3 normal, vec3 toEye);
vec3 computeLighting(AmbiantLight light);

void main()
{
	// Lights ------------------------------------
	PointLight light1;
	light1.position = vec4(view * vec4(0.5, 0, 0, 1)).xyz;
	light1.color = vec3(.2, 1, .2);
	light1.attenuation = 1;

	AmbiantLight light2;
	light2.color = vec3(1, 1, 1);
	light2.strength = .2;
	
	SpotLight light3;
    light3.position = vec4(view * vec4(1.2, 1.2, -.8, 1)).xyz;
    light3.color = vec3(1, .2, .2);
	light3.direction = mat3(view) * vec3(1, -1, 1);
	light3.spread = .9;
	light3.outerSpread = .8;
	light3.attenuation = .1;

	// Actual stuff ---------------------------
	vec3 position = texture(gPosition, texCoord0).rgb;
	vec3 toEye = normalize(vec3(0,0,0) - position);
	vec3 normal = texture(gNormal, texCoord0).rgb;

	if(normal == vec3(0,0,0)) {
		discard;
	}

	vec3 lighting = vec3(0,0,0);
	lighting += computeLighting(light1, position, normal, toEye);
	lighting += computeLighting(light2);
	lighting += computeLighting(light3, position, normal, toEye);
	
	gl_FragColor = vec4(lighting, 1) * texture(gAlbedo, texCoord0);
} 

vec3 computeLighting(PointLight light, vec3 position, vec3 normal, vec3 toEye) {
	vec3 lightDir = normalize(light.position - position);  
	float distance = length(light.position - position);

	// Diffuse
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * light.color;

	// Specular
	vec3 halfwayDir = normalize(lightDir + toEye);
	float spec = pow(max(dot(normal, halfwayDir), 0), 32);
	vec3 specular = 1 * spec * light.color;

	// Attenuation
	float attenuation = 1 + light.attenuation * distance * distance;

	return (diffuse + specular) / attenuation;
}

vec3 computeLighting(SpotLight light, vec3 position, vec3 normal, vec3 toEye) {
	vec3 lightDir = normalize(light.position - position);
	float distance = length(light.position - position);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.spread - light.outerSpread;
	float intensity = clamp((theta - light.outerSpread) / epsilon, 0.0, 1.0);    
	
	// Diffuse   
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * light.color * intensity;

	// Specular
	vec3 halfwayDir = normalize(lightDir + toEye);
	float spec = pow(max(dot(normal, halfwayDir), 0), 32);
	vec3 specular = 1 * spec * light.color;

	// Attenuation
	float attenuation = 1 + light.attenuation * distance * distance;

	return (diffuse + specular) / attenuation;
}

vec3 computeLighting(AmbiantLight light) {
	return light.color * light.strength * texture(ssaoLevel, texCoord0).r;
}