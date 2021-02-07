#version 330 core

in vec2 TexCoord; // We get texture coords from vert shader.
out vec4 fragColor;

in float fogAmount; // New: Fog amount for a vertex.
uniform vec3 FogColor; // New: Set color for global fog in program.

in vec3 Normal;
in vec3 FragPos;

// Draw call static variable.
uniform vec4 vertColor;


uniform vec3 viewPos; // Position of camera, for specular highlithing.


struct Light{
	vec3 ambient;
	vec3 position;
	vec3 diffuse;
	vec3 specular;

	float intensity;
};


struct DirLight{
	vec3 ambient;
	vec3 direction;
	vec3 diffuse;
	vec3 specular;

	float intensity;
};


struct SpotLight{

	vec3 ambient;
	vec3 position;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;

	float intensity;
};


struct PointLight{

	vec3 ambient;
	vec3 position;
	vec3 diffuse; // a.k.a color.
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float brightness;
};


struct Material{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};


uniform Light light;
uniform DirLight dirLight;
uniform SpotLight spotLight;

#define MAX_NUM_LIGHTS 32
uniform PointLight[MAX_NUM_LIGHTS] pointLight;
uniform int lightsInScene;

uniform Material material;


vec3 calculatePointLight(PointLight point_light, vec3 in_normal, vec3 frag_pos, vec3 view_dir);

void main(){


	// Check for Fog: if it is dominating, apply and return.
	if(fogAmount >= 1.0f){
		fragColor = vec4(FogColor, 1.0f);
		return;
	}



	vec3 resulting_color = vec3(0.0f);

	for(int i = 0; i < lightsInScene; i++){

		resulting_color += calculatePointLight(pointLight[i], Normal, FragPos, normalize(viewPos + FragPos));
	}

	//vec3 lightColor = calculatePointLight(pointLight, Normal, FragPos, normalize(viewPos + FragPos));

	// Calculate fogginess of  fragment.
	// Mix the color of fragment we calculated with the fog color...
	resulting_color = mix(resulting_color, FogColor, fogAmount);



	fragColor = vec4(resulting_color, 1.0f);
};



vec3 calculatePointLight(PointLight point_light, vec3 in_normal, vec3 frag_pos, vec3 view_dir){
	
	vec3 ambient = point_light.ambient * material.ambient* vec3(texture(material.diffuseMap, TexCoord)); // use here material.texture_diffuse1

	vec3 norm = normalize(Normal);

	vec3 lDir = normalize(point_light.position - frag_pos);


	float NDotL = max(dot(norm, lDir), 0.0f);

	vec3 diffuse = point_light.diffuse * vec3(texture(material.diffuseMap, TexCoord))*NDotL; // use here material.texture_diffuse1

	vec3 viewDir = normalize(viewPos - frag_pos);

	vec3 halfDir = normalize(lDir + viewDir);
	float NDotH = max(dot(norm, halfDir), 0.0f);

	vec3 specular = point_light.specular * point_light.specular* pow(NDotH, material.shininess); // use here material.texture_specular1

	// Applying equation for point light.
	float d = length(point_light.position - frag_pos); // magnitude between the 2 vecs.
	float attenuation = 1.0f / (point_light.constant + point_light.linear * d + point_light.quadratic * (d * d) ); // the formula..

	diffuse *= attenuation;
	specular *= attenuation;
	ambient *= attenuation;

	return vec3(point_light.brightness*ambient + point_light.brightness*diffuse + point_light.brightness*specular);
}
