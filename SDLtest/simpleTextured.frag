#version 400 core

in vec3 position0;
in vec3 normal0;
in vec3 tangent0;
in vec2 texCoord0;
in vec3 eyePos0;
in mat3 TBN;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;
uniform sampler2D textureBump;

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

vec3 computeNormal(vec2 texCoords);
vec3 computeLighting(PointLight light, vec3 normal, vec3 toEye);
vec3 computeLighting(SpotLight light, vec3 normal, vec3 toEye);
vec3 computeLighting(AmbiantLight light);
vec2 ParallaxMapping(vec3 toEye);

void main() {
	// Lights ------------------------------------
	PointLight light1;
	light1.position = vec3(0.5, 0, 0);
	light1.color = vec3(.2, 1, .2);
	light1.attenuation = 1;

	AmbiantLight light2;
	light2.color = vec3(1, 1, 1);
	light2.strength = .2;
	
	SpotLight light3;
    light3.position = vec3(1.2, 1.2, -.8);
    light3.color = vec3(1, .2, .2);
	light3.direction = vec3(1, -1, 1);
	light3.spread = .9;
	light3.outerSpread = .8;
	light3.attenuation = .1;

	// Actual stuff ---------------------------
	vec3 toEye = normalize(eyePos0 - position0);
	vec2 texCoords = ParallaxMapping(toEye);
	vec3 normal = computeNormal(texCoords);

	vec3 lighting = vec3(0,0,0);
	lighting += computeLighting(light1, normal, toEye);
	lighting += computeLighting(light2);
	lighting += computeLighting(light3, normal, toEye);
	
	gl_FragColor = vec4(lighting, 1) * texture(textureDiffuse, texCoords);
}

vec2 ParallaxMapping(vec3 toEye)
{ 
	toEye = inverse(TBN) * toEye;
	
    float numLayers = mix(30, 3, abs(dot(vec3(0, 0, 1), toEye)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0;
    vec2 P = toEye.xy / 32; 
    vec2 deltaTexCoords = P / numLayers;
	
	vec2  currentTexCoords     = texCoord0;
	float currentDepthMapValue = texture(textureBump, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(textureBump, currentTexCoords).r;
		currentLayerDepth += layerDepth;  
	}

	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(textureBump, prevTexCoords).r - currentLayerDepth + layerDepth;
 
	float weight = afterDepth / (afterDepth - beforeDepth);
	return prevTexCoords * weight + currentTexCoords * (1.0 - weight);
} 

vec3 computeNormal(vec2 texCoords) {
	vec3 normal = texture(textureNormal, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	return normalize(TBN * normal);
}

vec3 computeLighting(PointLight light, vec3 normal, vec3 toEye) {
	vec3 lightDir = normalize(light.position - position0);  
	float distance = length(light.position - position0);

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

vec3 computeLighting(SpotLight light, vec3 normal, vec3 toEye) {
	vec3 lightDir = normalize(light.position - position0);
	float distance = length(light.position - position0);
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
	return light.color * light.strength;
}