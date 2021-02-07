#version 330 core

in vec2 TexCoord; // We get texture coords from vert shader.
out vec4 fragColor;

in vec3 Normal;

// Draw call static variable.
uniform vec4 vertColor;


uniform sampler2D Texture;

uniform sampler2D Texture2; // For multitexturing.

//uniform vec3 lightColor; // For light color, we set it in program.

//uniform vec3 lightPos; // Position of light passed through program.

uniform vec3 viewPos; // Position of camera, for specular highlithing.


struct Material{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shineness;
};

struct SpotLight{
	vec3 ambient;
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;

	float constant;
	float linear;
	float exponent;


	float cosInnerCone;
	float cosOuterCone;
};



uniform SpotLight light;
uniform Material material;
uniform vec3 FragPos;

vec3 calcSpotLight();

void main(){

	// Adding phong light shader.
	// Ambient
	//float ambientFactor = 0.1f;
	//vec3 ambient = lightColor*ambientFactor;
	vec3 ambient = light.ambient * material.ambient *vec3(texture(material.diffuseMap, TexCoord));

	/*
	// Diffuse: LAmberts law.
	vec3 normal = normalize(Normal);

	vec3 ligthDir = normalize(light.position - FragPos);
	//vec3 ligthDir = normalize(-light.direction);


	float NDotL = max(dot(normal, ligthDir), 0.0f);

	//vec3 diffuse = lightColor*NDotL; // Light intensity.
	vec3 diffuse = light.diffuse* vec3(texture(material.diffuseMap, TexCoord))*NDotL;

	// Specular (Phong).
	//float shineness = 80.0f;
	//float specularFactor = 0.8f;
	vec3 viewDir = normalize(viewPos - FragPos);

	// Adding Blinn-Phong lighting model.
	vec3 halfDir = normalize(ligthDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);

	//vec3 reflectDir = reflect(-ligthDir, normal);
	
	//float RDotV = max(dot(reflectDir, viewDir), 0.0f); // max of Reflection dot viewDir and 0.
	//vec3 specular = lightColor * specularFactor * pow(NDotV, shineness);
	vec3 specular = light.specular * material.specular* pow(NDotH, material.shineness);


	// Applying equation for point light.
	float d = length(light.position - FragPos); // magnitude between the 2 vecs.
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d) ); // the formula..

	diffuse *= attenuation; // use formula.
	specular *= attenuation;
	*/

	// Spot light.
	vec3 spotColor = vec3(0.0);


		vec3 ligthDir = normalize(light.position - FragPos);
	//vec3 ambient = light.ambient * material.ambient *vec3(texture(material.diffuseMap, TexCoord));
	vec3 spotDir = normalize(light.direction);



	float cosDir = dot(-light.direction, spotDir); // Direction of light should be inverted...
	


	float spotIntensity = smoothstep(light.cosOuterCone, light.cosInnerCone, cosDir); // Make gradient for inner and outer cone of spot light.
	
	// Diffuse: LAmberts law.
	vec3 normal = normalize(Normal);

	//vec3 ligthDir = normalize(-light.direction);


	float NDotL = max(dot(normal, ligthDir), 0.0f);

	//vec3 diffuse = lightColor*NDotL; // Light intensity.
	vec3 diffuse = light.diffuse* vec3(texture(material.diffuseMap, TexCoord))*NDotL;

	// Specular (Phong).
	//float shineness = 80.0f;
	//float specularFactor = 0.8f;
	vec3 viewDir = normalize(viewPos - FragPos);

	// Adding Blinn-Phong lighting model.
	vec3 halfDir = normalize(ligthDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);

	//vec3 reflectDir = reflect(-ligthDir, normal);
	
	//float RDotV = max(dot(reflectDir, viewDir), 0.0f); // max of Reflection dot viewDir and 0.
	//vec3 specular = lightColor * specularFactor * pow(NDotV, shineness);
	vec3 specular = light.specular * material.specular* pow(NDotH, material.shineness);


	// Applying equation for point light.
	float d = length(light.position - FragPos); // magnitude between the 2 vecs.
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d) ); // the formula..

	diffuse *= attenuation * spotIntensity; // use formula.
	specular *= attenuation * spotIntensity;

	spotColor = attenuation + specular;

	//vec4 texel = texture(Texture, TexCoord);
	fragColor = vec4(ambient + spotColor, 1.0); // Applying Phong model.


	//fragColor = texture(Texture, TexCoord); // Map texture colors to color of pixels.
	//fragColor = mix(texture(Texture, TexCoord), texture(Texture2, TexCoord), 0.0); // Interpolate between the 2 textures for a new color.
};



vec3 calcSpotLight(){

	vec3 ligthDir = normalize(light.position - FragPos);
	//vec3 ambient = light.ambient * material.ambient *vec3(texture(material.diffuseMap, TexCoord));
	vec3 spotDir = normalize(light.direction);



	float cosDir = dot(-light.direction, spotDir); // Direction of light should be inverted...
	


	float spotIntensity = smoothstep(light.cosOuterCone, light.cosInnerCone, cosDir); // Make gradient for inner and outer cone of spot light.
	
	// Diffuse: LAmberts law.
	vec3 normal = normalize(Normal);

	//vec3 ligthDir = normalize(-light.direction);


	float NDotL = max(dot(normal, ligthDir), 0.0f);

	//vec3 diffuse = lightColor*NDotL; // Light intensity.
	vec3 diffuse = light.diffuse* vec3(texture(material.diffuseMap, TexCoord))*NDotL;

	// Specular (Phong).
	//float shineness = 80.0f;
	//float specularFactor = 0.8f;
	vec3 viewDir = normalize(viewPos - FragPos);

	// Adding Blinn-Phong lighting model.
	vec3 halfDir = normalize(ligthDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);

	//vec3 reflectDir = reflect(-ligthDir, normal);
	
	//float RDotV = max(dot(reflectDir, viewDir), 0.0f); // max of Reflection dot viewDir and 0.
	//vec3 specular = lightColor * specularFactor * pow(NDotV, shineness);
	vec3 specular = light.specular * material.specular* pow(NDotH, material.shineness);


	// Applying equation for point light.
	float d = length(light.position - FragPos); // magnitude between the 2 vecs.
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d) ); // the formula..

	diffuse *= attenuation * spotIntensity; // use formula.
	specular *= attenuation * spotIntensity;

	return (diffuse + specular);
};