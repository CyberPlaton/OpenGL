#include"Sprite.h"

Texture2D::Texture2D() : m_TextureHandle(0) {

}

Texture2D::~Texture2D() {


}


bool Texture2D::LoadTexture(const std::string& fileName, bool genMipMaps) {

	int w, h, cmp; // cmp = rgb or rgba etc.

	// Load image.
	unsigned char* imgData = stbi_load(fileName.c_str(), &w, &h, &cmp, STBI_rgb_alpha);

	if (imgData != NULL) {

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

void Texture2D::Bind(GLuint texUint) {


	glActiveTexture(GL_TEXTURE0 + texUint); // For binding texture to "texUint" particular index.

	glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
}

void Texture2D::Unbind(GLuint texUint) {

	glActiveTexture(GL_TEXTURE0 + texUint); // Bind to textures index a 0 texture --> unbind.
	glBindTexture(GL_TEXTURE_2D, 0);
}


Sprite::Sprite(std::string shader, std::string texture) {

	m_SpriteShader = new ShaderProgram();
	m_SpriteShader->LoadShaders((shader + ".vert").c_str(), (shader + ".frag").c_str());

	m_SpriteTexture = new Texture2D();
	m_SpriteTexture->LoadTexture(texture);





	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(m_QuadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


Sprite::~Sprite() {

}


void Sprite::Draw(){

	m_SpriteShader->Use();

	m_SpriteTexture->Bind(); // Equals to setting uniform sampler...

	glBindVertexArray(m_QuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);


	m_SpriteTexture->Unbind();
}