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



void ParticleSystem::emit() {

	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.m_Active = true;
	particle.m_Position = m_ParticleData->m_Position;
	particle.m_Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.m_Velocity = m_ParticleData->m_Velocity;
	particle.m_Velocity.x += m_ParticleData->m_VelocityVariation.x * (Random::Float() - 0.5f);
	particle.m_Velocity.y += m_ParticleData->m_VelocityVariation.y * (Random::Float() - 0.5f);
	
	// Color
	particle.m_ColorBegin = m_ParticleData->m_ColorBegin;
	particle.m_ColorEnd = m_ParticleData->m_ColorEnd;

	particle.m_LifeTime = m_ParticleData->m_LifeTime;
	particle.m_LifeRemaining = m_ParticleData->m_LifeTime;
	particle.m_SizeBegin = m_ParticleData->m_SizeBegin + m_ParticleData->m_SizeVariation * (Random::Float() - 0.5f);
	particle.m_SizeEnd = m_ParticleData->m_SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

ParticleSystem* ParticleSystem::createFromFile(std::string file) {

	using namespace std;

	Sprite* sprite = nullptr;
	ParticleSystem* ps = nullptr;

	try {

		YAML::Node node = YAML::LoadFile(file);

		// Create Particle system.
		sprite = new Sprite("shaderTexture", node["texture"].as<std::string>());
		ps = new ParticleSystem(sprite);

		// Initialize data for population.
		ps->m_ParticleData = new ParticleData();

		ps->m_ParticleData->m_Position = glm::vec2(node["position"][0].as<float>(), node["position"][1].as<float>());

		ps->m_ParticleData->m_Velocity = glm::vec2(node["velocity"][0].as<float>(), node["velocity"][1].as<float>());

		ps->m_ParticleData->m_VelocityVariation = glm::vec2(node["velocityVar"][0].as<float>(), node["velocityVar"][1].as<float>());

		ps->m_ParticleData->m_ColorBegin = glm::vec4(node["colorBegin"][0].as<float>(), node["colorBegin"][1].as<float>(), node["colorBegin"][2].as<float>(), node["colorBegin"][3].as<float>());

		ps->m_ParticleData->m_ColorEnd = glm::vec4(node["colorEnd"][0].as<float>(), node["colorEnd"][1].as<float>(), node["colorEnd"][2].as<float>(), node["colorEnd"][3].as<float>());

		ps->m_ParticleData->m_LifeTime = node["lifeTime"].as<float>();

		ps->m_ParticleData->m_SizeBegin = node["sizeBegin"].as<float>();

		ps->m_ParticleData->m_SizeEnd = node["sizeEnd"].as<float>();

		ps->m_ParticleData->m_SizeVariation = node["sizeVar"].as<float>();


		return ps;
	}
	catch (std::exception e) {
		cout << color(colors::RED);
		cout << "Exception: " << e.what() << "\nin (ParticleSystem::createFromFile) \n" << white;

		delete sprite;
		delete ps;
		return nullptr;
	}



	/*
	cout << color(colors::RED);
	cout << "position: " << node["position"][0].as<float>() << node["position"][1].as<float>() << endl;
	cout << "velocity: " << node["velocity"][0].as<float>() << node["velocity"][1].as<float>() << endl;
	cout << "velocityVar: " << node["velocityVar"][0].as<float>() << node["velocityVar"][1].as<float>() << endl;
	cout << "colorBegin: " << node["colorBegin"][0].as<float>() << node["colorBegin"][1].as<float>() << node["colorBegin"][2].as<float>() << node["colorBegin"][3].as<float>() << endl;
	cout << "colorEnd: " << node["colorEnd"][0].as<float>() << node["colorEnd"][1].as<float>() << node["colorEnd"][2].as<float>() << node["colorEnd"][3].as<float>() << endl;
	cout << "lifeTime: " << node["lifeTime"].as<float>() << endl;
	cout << "sizeBegin: " << node["sizeBegin"].as<float>() << endl;
	cout << "sizeEnd: " << node["sizeEnd"].as<float>() << endl;
	cout << "sizeVar: " << node["sizeVar"].as<float>() << endl;
	cout << "texture: " << node["texture"].as<std::string>() << endl;
	*/

	/*
	cout << color(colors::CYAN);
	cout << "host: " << config["host"].as<std::string>() << endl;
	cout << "port: " << config["port"].as<int>() << endl;
	cout << "password: " << config["password"].as<std::string>() << endl;
	cout << "servicename: " << config["servicename"].as<std::string>() << white << endl;
	*/
}
