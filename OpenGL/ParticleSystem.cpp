#include"ParticleSystem.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;


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


ParticleSystem::ParticleSystem(int screenwidth, int screenheight){

	m_ParticlePool.resize(1000);

	m_ProjectionMatrix = glm::ortho(0.0f, (float)screenwidth, (float)screenheight, 0.0f, 0.1f, 1000.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * glm::mat4(1.0f);

}



void ParticleSystem::onUpdate(float dt, glm::vec3 camerPos) {

	// Uupdate view projection mat.
	// For testing rotation is some default value...
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), camerPos) * glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0, 0, 1));
	m_ViewProjectionMatrix = m_ProjectionMatrix * glm::inverse(transform);


	for (auto& particle : m_ParticlePool)
	{
		if (!particle.m_Active)
			continue;

		if (particle.m_LifeRemaining <= 0.0f)
		{
			particle.m_Active = false;
			continue;
		}

		particle.m_LifeRemaining -= dt;
		particle.m_Position += particle.m_Velocity * (float)dt;
		particle.m_Rotation += 0.01f * dt;
	}
}

void ParticleSystem::onRender(glm::mat4 projection, glm::mat4 view, ShaderProgram* shader) {



	if (!m_QuadVAO)
	{
		float vertices[] = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f
		};


		// Send vetrices to GPU for storing.
		glGenBuffers(1, &m_QuadVBO); // Create chunk of memory in graphics card.
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO); // Make this buffer the active one.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Pass mesh data to active buffer.

		// Create vertex array object, which holds information of vertex buffer object.
		glGenVertexArrays(1, &m_QuadVAO); // Create..
		glBindVertexArray(m_QuadVAO);     // Make it the active...

		// Describe usage of above vertex object for GPU.
		// Position attributes.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, NULL);
		glEnableVertexAttribArray(0); // Enable vertex array object.


		/*
		GLuint quadIB;
		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		*/

		/*
		glCreateVertexArrays(1, &m_QuadVA);
		glBindVertexArray(m_QuadVA);

		GLuint quadVB, quadIB;
		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(quadVB, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		*/

		/*
		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		*/

		/*
		m_ParticleShader = std::unique_ptr<GLCore::Utils::Shader>(GLCore::Utils::Shader::FromGLSLTextFiles("assets/shader.glsl.vert", "assets/shader.glsl.frag"));
		m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_ViewProj");
		m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Transform");
		m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Color");
		*/
	}







	for (auto& particle : m_ParticlePool)
	{
		if (!particle.m_Active)
			continue;


		shader->Use();

		// Fade away particles
		float life = particle.m_LifeRemaining / particle.m_LifeTime;
		//glm::vec4 color = glm::lerp(particle.m_ColorEnd, particle.m_ColorBegin, life);
		glm::vec4 color = glm::mix(particle.m_ColorEnd, particle.m_ColorBegin, life);
		//color.a = color.a * life;

		//float size = glm::lerp(particle.m_SizeEnd, particle.m_SizeBegin, life);
		float size = glm::mix(particle.m_SizeEnd, particle.m_SizeBegin, life);

		// Render
		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(particle.m_Position.x, particle.m_Position.y, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(size, size, size));

		/*
		glm::mat4 transform = glm::translate(glm::mat4(), { particle.m_Position.x, particle.m_Position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.m_Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		*/ 
			
		shader->SetUniform("viewProjection", m_ViewProjectionMatrix);
		shader->SetUniform("model", model);
		shader->SetUniform("color", color);
		

		glBindVertexArray(m_QuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);


		glEnable(GL_CULL_FACE);
		glEnable(GL_CCW);
		glEnable(GL_BLEND); // Enable for fading effect...
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0); // Unbind after drawing...
	}
}



void ParticleSystem::emit(const ParticleData& props) {

	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.m_Active = true;
	particle.m_Position = props.m_Position;
	particle.m_Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.m_Velocity = props.m_Velocity;
	particle.m_Velocity.x += props.m_VelocityVariation.x * (Random::Float() - 0.5f);
	particle.m_Velocity.y += props.m_VelocityVariation.y * (Random::Float() - 0.5f);
	
	// Color
	particle.m_ColorBegin = props.m_ColorBegin;
	particle.m_ColorEnd = props.m_ColorEnd;

	particle.m_LifeTime = props.m_LifeTime;
	particle.m_LifeRemaining = props.m_LifeTime;
	particle.m_SizeBegin = props.m_SizeBegin + props.m_SizeVariation * (Random::Float() - 0.5f);
	particle.m_SizeEnd = props.m_SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}


/*
ParticleSystem::ParticleSystem(std::string texturePath, unsigned int particleCount,
    glm::vec4 color, glm::vec2 position, glm::vec2 velocity, float lifespan, float lifeVariance){

	m_ParticleCount = particleCount;
	m_ParticleTexture = new Texture2D();
	m_ParticleTexture->LoadTexture(texturePath);

	for (int i = 0; i < particleCount; i++) {

		m_ParticlePool.push_back(new ParticleData(position, velocity, color, lifespan));
	}

	_loadShader();


	GLuint VBO;
	float particle_quad[] = {
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

}





void ParticleSystem::Update(glm::vec2 objectPos, float dt){

	unsigned int nr_new_particles = 2;
	// add new particles
	for (unsigned int i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = _firstUnusedParticle();
		_respawnParticle(*m_ParticlePool[unusedParticle], objectPos, glm::vec2(1.0f));
	}
	// update all particles
	for (unsigned int i = 0; i < m_ParticleCount; ++i)
	{
		ParticleData& p = *m_ParticlePool[i];
		p.m_LifeSpan -= dt; // reduce life
		if (p.m_LifeSpan > 0.0f)
		{	// particle is alive, thus update
			p.m_Position -= p.m_Velocity * dt;
			p.m_Color.a -= dt * 2.5f;
		}
	}



	// Draw particles.
	_draw();
}

unsigned int  ParticleSystem::_firstUnusedParticle() {

	// search from last used particle, this will usually return almost instantly
	for (unsigned int i = m_LastUsedParticle; i < m_ParticleCount; ++i) {
		if (m_ParticlePool[i]->m_LifeSpan <= 0.0f) {
			m_LastUsedParticle = i;
			return i;
		}
	}
	// otherwise, do a linear search
	for (unsigned int i = 0; i < m_LastUsedParticle; ++i) {
		if (m_ParticlePool[i]->m_LifeSpan <= 0.0f) {
			m_LastUsedParticle = i;
			return i;
		}
	}
	// override first particle if all others are alive
	m_LastUsedParticle = 0;
	return 0;
}


void ParticleSystem::_respawnParticle(ParticleData& particle, glm::vec2& objectPos, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.m_Position = objectPos + random + offset;
	particle.m_Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.m_LifeSpan = 1.0f;
	particle.m_Velocity = glm::vec2(1.0f * 0.1f);
}

void ParticleSystem::_draw(){

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_ParticleShader->Use();

	for (ParticleData* p : m_ParticlePool) {

		if (p->m_LifeSpan > 0.0f) {

			m_ParticleShader->SetUniform("offset", p->m_Position);
			m_ParticleShader->SetUniform("color", p->m_Color);
			m_ParticleShader->SetUniform("projection", m_Projection);

			m_ParticleTexture->Bind();
			m_ParticleShader->SetUniformSampler("sprite", m_ParticleTexture->GetTextureHandle());

			glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			m_ParticleTexture->Unbind();
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_ParticleTexture->Unbind();

}


bool ParticleSystem::_loadShader(){

	m_ParticleShader = new ShaderProgram();
	m_ParticleShader->LoadShaders(m_DefaultShaderVert.c_str(), m_DefaultShaderFrag.c_str());

	return m_ParticleShader;
}
*/