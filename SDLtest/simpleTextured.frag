#version 400 core

in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 toLight;
in vec3 toCamera;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;

void main() {
	vec3 unitTangent = normalize(tangent);
	vec3 unitNormal = normalize(normal);
	vec3 unitToLight = normalize(toLight);
	vec3 unitToCamera = normalize(toCamera);

	// Normal
	unitTangent = normalize(unitTangent - dot(unitTangent, unitNormal) * unitNormal);
	vec3 unitBitangent = cross(unitTangent, unitNormal);
	vec3 bumpNormal = texture(textureNormal, texCoord).xyz;
	bumpNormal = vec3(bumpNormal.x *2 -1, bumpNormal.y *2 -1, bumpNormal.z);

	mat3 TBN = mat3(unitTangent, unitBitangent, unitNormal);
	vec3 NewNormal = normalize(TBN * bumpNormal);

	// Material
	vec3 materialColor = vec3(1, 0.87, 0.51);
	float shineDamper = 50;
	float reflectivity = .8;
	
	// Diffuse 
	vec3 ambiantColor = vec3(1, 1, 1);
	float ambiantBrightness = .2;

	vec3 lightColor = vec3(1, 1, 1);
	float lightBrightness = max(dot(NewNormal, unitToLight), 0.0);

	vec3 diffuse = lightColor * lightBrightness + ambiantColor * ambiantBrightness;

	// Specular
	vec3 reflectedLight = reflect(-unitToLight, NewNormal);
	float specularFactor = max(dot(reflectedLight, unitToCamera), 0.0);
	vec3 specular = pow(specularFactor, shineDamper) * lightColor;

    gl_FragColor = texture(textureDiffuse, texCoord) * vec4(diffuse, 1)  + vec4(specular, 1) * reflectivity;
}