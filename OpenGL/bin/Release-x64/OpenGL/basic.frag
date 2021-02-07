#version 330 core

//in vec2 TexCoord; // We get texture coords from vert shader.
out vec4 fragColor;

uniform vec3 lightColor;

// Draw call static variable.
uniform vec4 vertColor;


uniform sampler2D Texture;

uniform sampler2D Texture2; // For multitexturing.

void main(){

	//fragColor = texture(Texture, TexCoord); // Map texture colors to color of pixels.
	//fragColor = mix(texture(Texture, TexCoord), texture(Texture2, TexCoord), 0.0); // Interpolate between the 2 textures for a new color.
	fragColor = vec4(lightColor, 1.0f);
};