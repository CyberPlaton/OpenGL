#include"Texture2D.h"

/*
=======
#define STB_IMAGE_IMPLEMENTATION // Needed for library.
#include<stb_image/stb_image.h>

>>>>>>> c2ab18a67873b78bb3cdab0710904f17845befa4
Texture2D::Texture2D() : m_TextureHandle(0) {

}

Texture2D::~Texture2D() {


}


bool Texture2D::LoadTexture(const std::string& fileName, bool genMipMaps) {

	int w, h, cmp; // cmp = rgb or rgba etc.

	// Load image.
	unsigned char* imgData = stbi_load(fileName.c_str(), &w, &h, &cmp, STBI_rgb_alpha);

	if (imgData != NULL) {

<<<<<<< HEAD
=======
		// Invrt image orientation.
		int byteWidth = w * 4;
		unsigned char* top = NULL;
		unsigned char* bottom = NULL;
		unsigned char temp = NULL;
		int halfHeight = h / 2;
		for (int row = 0; row < halfHeight; row++) {
			top = imgData + row * byteWidth;
			bottom = imgData + (h - row - 1) * byteWidth;
			for (int col = 0; col < byteWidth; col++) {
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				top++;
				bottom++;
			}
		}


>>>>>>> c2ab18a67873b78bb3cdab0710904f17845befa4
		// Create texture object.
		glGenTextures(1, &m_TextureHandle);


		glBindTexture(GL_TEXTURE_2D, m_TextureHandle);


		// Apply options for texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // Up axis for OpenGL texels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Down axis.

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear texture filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Map bits from image to opengl texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		// Generate mip maps.
		if (genMipMaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Free memory of image.
		stbi_image_free(imgData);

		// Free binding for image = unbind. 
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	else {

		using namespace std;
		cout << color(colors::RED);
		cout << "Error Loading image: " << fileName << white << endl;
		return false;
	}
}

void Texture2D::Bind(GLuint texUint){


	glActiveTexture(GL_TEXTURE0 + texUint); // For binding texture to "texUint" particular index.

	glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
}

void Texture2D::Unbind(GLuint texUint) {

	glActiveTexture(GL_TEXTURE0 + texUint); // Bind to textures index a 0 texture --> unbind.
	glBindTexture(GL_TEXTURE_2D, 0);
<<<<<<< HEAD
}
*/
