#version 330 core

layout (location = 0) in vec3 pos;

layout(location = 1) in vec3 normal; // Normals.

layout (location = 2) in vec2 texCoord; // Texture.



uniform mat4 model; // Matrices...
uniform mat4 view; 
uniform mat4 projection; 

uniform mat4 world = mat4(1.0); // Identity.

uniform float FogStart = 1000.0f;
uniform float FogEnd = 10000.0f;

uniform vec3 CameraPosition; // New: must be set in program.

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

out float fogAmount;

void main()
{
	Normal = normal;
	FragPos = vec3(model*vec4(pos, 1.0f)); // Position of incomming vertex position but in WORLD POSITION = normalized device coords.

	gl_Position = projection*view*model*vec4(pos.x, pos.y, pos.z, 1.0f); // Create world coords for cube...

	TexCoord = texCoord;


	// Get position of fragment in the world.
	vec3 worldPos = (world * vec4(pos, 1.0f)).xyz; // .xyz mean we convert from 4vec to 3vec

	// Calculate its fog amount.
	fogAmount = clamp(( distance(worldPos, CameraPosition) - FogStart), 0.0f, 1.0f);
};