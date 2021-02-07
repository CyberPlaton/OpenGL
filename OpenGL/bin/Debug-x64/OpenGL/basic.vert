#version 330 core

layout (location = 0) in vec3 pos;

layout(location = 1) in vec3 normal; // Normals.

layout (location = 2) in vec2 texCoord; // Texture.

uniform mat4 model; // Matrices...
uniform mat4 view; 
uniform mat4 projection; 

// adding draw call static variable.
// A uniform var is seen across all shaders in a draw call.
uniform vec2 posOffset;

out vec2 TexCoord;

void main()
{

	//gl_Position = vec4(pos.x + posOffset.x, pos.y + posOffset.y, pos.z, 1.0f);

	gl_Position = projection*view*model*vec4(pos.x, pos.y, pos.z, 1.0f); // Create world coords for cube...

	TexCoord = texCoord;
};