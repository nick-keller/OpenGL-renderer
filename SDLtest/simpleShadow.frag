#version 400 core

in vec3 normal;
in vec3 toLight;
in vec3 toCamera;


void main() {
	vec3 unitNormal = normalize(normal);
	vec3 unitToLight = normalize(toLight);
	vec3 unitToCamera = normalize(toCamera);

	// Material
	vec3 materialColor = vec3(1, 0.87, 0.51);
	float shineDamper = 50;
	float reflectivity = .8;
	
	// Diffuse 
	vec3 ambiantColor = vec3(1, 1, 1);
	float ambiantBrightness = .2;

	vec3 lightColor = vec3(1, 1, 1);
	float lightBrightness = max(dot(unitNormal, unitToLight), 0.0);

	vec3 diffuse = lightColor * lightBrightness + ambiantColor * ambiantBrightness;

	// Specular
	vec3 reflectedLight = reflect(-unitToLight, unitNormal);
	float specularFactor = max(dot(reflectedLight, unitToCamera), 0.0);
	vec3 specular = pow(specularFactor, shineDamper) * lightColor;

    gl_FragColor = vec4(materialColor * diffuse + specular * reflectivity, 1);
}