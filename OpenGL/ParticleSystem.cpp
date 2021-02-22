#include"ParticleSystem.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;


ParticleSystem::ParticleSystem(Sprite* sprite){

	m_ParticlePool.resize(1000);

	m_ParticleSprite = sprite;
}



void ParticleSystem::onUpdate(float dt, glm::vec3 camerPos) {

	
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

void ParticleSystem::onRender() {


	for (auto& particle : m_ParticlePool)
	{
		if (!particle.m_Active)
			continue;

		// Fade away particles
		float life = particle.m_LifeRemaining / particle.m_LifeTime;
		//glm::vec4 color = glm::lerp(particle.m_ColorEnd, particle.m_ColorBegin, life);
		glm::vec4 color = glm::mix(particle.m_ColorEnd, particle.m_ColorBegin, life);
		//color.a = color.a * life;

		//float size = glm::lerp(particle.m_SizeEnd, particle.m_SizeBegin, life);
		float size = glm::mix(particle.m_SizeEnd, particle.m_SizeBegin, life);


		m_ParticleSprite->SetColor(glm::vec3(color.x, color.y, color.z));
		m_ParticleSprite->SetBrightness(color.a);
		m_ParticleSprite->SetScale(size);
		m_ParticleSprite->SetPosition(particle.m_Position);

		m_ParticleSprite->Draw();
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